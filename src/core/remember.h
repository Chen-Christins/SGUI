#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace sgui {

// Reactive mutable state that triggers invalidate on set().
// Designed to be stored in CompositionContext::rememberState().
template<typename T>
class MutableState {
public:
    MutableState(T initial, std::shared_ptr<std::function<void()>> onInvalidate)
        : value_(std::move(initial)), onInvalidate_(std::move(onInvalidate)) {
    }

    const T& get() const {
        return value_;
    }

    void set(T newValue) {
        value_ = std::move(newValue);
        if (*onInvalidate_) {
            (*onInvalidate_)();
        }
    }

private:
    T value_;
    std::shared_ptr<std::function<void()>> onInvalidate_;
};

// Persists values across full widget tree rebuilds.
class CompositionContext {
public:
    CompositionContext()
        : onInvalidate_(std::make_shared<std::function<void()>>()) {
    }

    explicit CompositionContext(std::function<void()> onInvalidate)
        : onInvalidate_(std::make_shared<std::function<void()>>(std::move(onInvalidate))) {
    }

    void setInvalidate(std::function<void()> fn) {
        *onInvalidate_ = std::move(fn);
    }

    void reset() {
        nextPosition_ = 0;
    }

    template<typename T>
    T& remember(std::function<T()> factory) {
        int pos = nextPosition_++;
        auto it = store_.find(pos);
        if (it != store_.end()) {
            return *static_cast<T*>(it->second.get());
        }
        auto value = std::make_shared<T>(factory());
        T* ptr = value.get();
        store_[pos] = std::move(value);
        return *ptr;
    }

    template<typename T>
    MutableState<T>& rememberState(std::function<T()> factory) {
        int pos = nextPosition_++;
        auto it = stateStore_.find(pos);
        if (it != stateStore_.end()) {
            return *static_cast<MutableState<T>*>(it->second.get());
        }
        auto ms = std::make_shared<MutableState<T>>(factory(), onInvalidate_);
        MutableState<T>* ptr = ms.get();
        stateStore_[pos] = std::move(ms);
        return *ptr;
    }

    template<typename T>
    T& rememberKey(const std::string& key, std::function<T()> factory) {
        auto it = keyStore_.find(key);
        if (it != keyStore_.end()) {
            return *static_cast<T*>(it->second.get());
        }
        auto value = std::make_shared<T>(factory());
        T* ptr = value.get();
        keyStore_[key] = std::move(value);
        return *ptr;
    }

private:
    int nextPosition_ = 0;
    std::shared_ptr<std::function<void()>> onInvalidate_;
    std::unordered_map<int, std::shared_ptr<void>> store_;
    std::unordered_map<int, std::shared_ptr<void>> stateStore_;
    std::unordered_map<std::string, std::shared_ptr<void>> keyStore_;
};

} // namespace sgui
