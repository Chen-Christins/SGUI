#pragma once

#include "remember.h"
#include "widget.h"
#include <string>
#include <vector>

namespace sgui {

class Dropdown : public Widget {
public:
    Dropdown(MutableState<int>& selectedIndex, std::vector<std::string> options,
             bool& isOpen);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    MutableState<int>& selectedIndex_;
    std::vector<std::string> options_;
    bool& isOpen_;

    static constexpr int kButtonWidth = 200;
    static constexpr int kButtonHeight = 40;
    static constexpr int kItemHeight = 32;
};

} // namespace sgui
