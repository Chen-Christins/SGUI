#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "widget.h"

namespace sgui {

// Helper functions to create widgets in a declarative way
inline std::shared_ptr<Widget> text(const std::string& str) {
    return std::make_shared<Text>(str);
}

inline std::shared_ptr<Widget> button(const std::string& str, std::function<void()> onClick) {
    return std::make_shared<Button>(str, std::move(onClick));
}

inline std::shared_ptr<Widget> column(std::vector<std::shared_ptr<Widget>> children) {
    return std::make_shared<Column>(std::move(children));
}

} // namespace sgui
