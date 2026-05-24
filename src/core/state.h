#pragma once

#include <functional>
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

    const T& get() const {
        return value_;
    }

    void observe(Observer observer) {
        observers_.push_back(observer);
        // Initial notify to the new observer
        observer(value_);
    }

private:
    void notify() {
        for (auto& obs : observers_) {
            obs(value_);
        }
    }

    T value_;
    std::vector<Observer> observers_;
};

} // namespace sgui
