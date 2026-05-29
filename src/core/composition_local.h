#pragma once

#include "widget.h"
#include <any>
#include <stack>
#include <unordered_map>

namespace sgui {

// Base type for type-erased CompositionLocal key storage
struct CompositionLocalKey {
    virtual ~CompositionLocalKey() = default;
};

// Thread-local scope stack for CompositionLocal values during render
class CompositionLocalScope {
public:
    static CompositionLocalScope& instance() {
        thread_local CompositionLocalScope scope;
        return scope;
    }

    template <typename T>
    void push(CompositionLocalKey* key, T value) {
        auto& stack = stacks_[key];
        stack.push(std::make_any<T>(value));
    }

    void pushAny(CompositionLocalKey* key, std::any value) { stacks_[key].push(std::move(value)); }

    void pop(CompositionLocalKey* key) {
        auto it = stacks_.find(key);
        if (it != stacks_.end() && !it->second.empty()) {
            it->second.pop();
        }
    }

    template <typename T>
    T get(CompositionLocalKey* key, const T& defaultVal) const {
        auto it = stacks_.find(key);
        if (it != stacks_.end() && !it->second.empty()) {
            return std::any_cast<T>(it->second.top());
        }
        return defaultVal;
    }

private:
    std::unordered_map<CompositionLocalKey*, std::stack<std::any>> stacks_;
};

// Pushes a CompositionLocal value during render and pops it after.
class ProviderModifier : public Widget {
public:
    ProviderModifier(CompositionLocalKey* key, std::any value, std::shared_ptr<Widget> child)
        : key_(key), value_(std::move(value)), child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        CompositionLocalScope::instance().pushAny(key_, value_);
        child_->render(ctx);
        CompositionLocalScope::instance().pop(key_);
    }

    Size measure() const override { return child_->measure(); }

private:
    CompositionLocalKey* key_;
    std::any value_;
    std::shared_ptr<Widget> child_;
};

// Provides implicit value propagation through the widget tree.
// Usage:
//   CompositionLocal<Color> primaryColor{RED};
//   auto current = primaryColor.current();                  // read
//   primaryColor.provides(BLUE, someChild);                 // provide for subtree
template <typename T>
class CompositionLocal : public CompositionLocalKey {
public:
    explicit CompositionLocal(T defaultValue) : defaultValue_(std::move(defaultValue)) {}

    T current() const {
        return CompositionLocalScope::instance().get<T>(const_cast<CompositionLocal*>(this), defaultValue_); 
    }

    std::shared_ptr<Widget> provides(T value, std::shared_ptr<Widget> child) const {
        return std::make_shared<ProviderModifier>(const_cast<CompositionLocal*>(this), std::any(std::move(value)), std::move(child));
    }

private:
    T defaultValue_;
};

} // namespace sgui
