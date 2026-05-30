#pragma once

#include <functional>
#include <unordered_set>
#include <vector>

namespace sgui {

// A simple reactive state container
template <typename T>
class State {
public:
    using Observer = std::function<void(const T&)>;

    State(T initial_value) : value_(std::move(initial_value)) {}

    void set(T new_value) {
        value_ = std::move(new_value);
        notify();
    }

    const T& get() const { return value_; }

    void observe(Observer observer) {
        observers_.push_back(observer);
        // Initial notify to the new observer
        observer(value_);
    }

private:
    void notify() {
        // Snapshot to allow observers to modify the list during iteration
        auto snapshot = observers_;
        for (auto& obs : snapshot) {
            obs(value_);
        }
    }

    T value_;
    std::vector<Observer> observers_;
};

// A lazy computed value that caches until explicitly invalidated.
// Dependencies are declared manually via dependsOn().
template <typename T>
class DerivedState {
public:
    using Observer = std::function<void(const T&)>;

    template <typename F>
    DerivedState(F compute) : compute_(compute), dirty_(true) {}

    const T& get() {
        if (dirty_) {
            cachedValue_ = compute_();
            dirty_ = false;
        }
        return cachedValue_;
    }

    void invalidate() {
        if (!dirty_) {
            dirty_ = true;
            notify();
        }
    }

    template <typename U>
    void dependsOn(State<U>& state) {
        auto key = reinterpret_cast<uintptr_t>(&state);
        if (deps_.find(key) != deps_.end()) {
            return;
        }
        deps_.insert(key);
        state.observe([this](const U&) { invalidate(); });
    }

    void observe(Observer observer) { observers_.push_back(std::move(observer)); }

private:
    void notify() {
        const T& val = get();
        auto snapshot = observers_;
        for (auto& obs : snapshot) {
            obs(val);
        }
    }

    std::function<T()> compute_;
    T cachedValue_{};
    bool dirty_ = true;
    std::vector<Observer> observers_;
    std::unordered_set<uintptr_t> deps_;
};

// Factory for DerivedState
template <typename F>
auto derivedStateOf(F compute) -> DerivedState<decltype(compute())> {
    return DerivedState<decltype(compute())>(compute);
}

} // namespace sgui
