#pragma once

#include "remember.h"
#include "widget.h"
#include <memory>
#include <string>
#include <vector>

namespace sgui {

class Dialog : public Widget {
public:
    Dialog(MutableState<bool>& isOpen, std::string title,
           std::shared_ptr<Widget> content,
           std::vector<std::string> buttonLabels,
           MutableState<int>& buttonClicked);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    MutableState<bool>& isOpen_;
    std::string title_;
    std::shared_ptr<Widget> content_;
    std::vector<std::string> buttonLabels_;
    MutableState<int>& buttonClicked_;

    static constexpr int kPanelWidth = 400;
    static constexpr int kTitleHeight = 42;
    static constexpr int kButtonHeight = 36;
    static constexpr int kPadding = 16;
    static constexpr int kCornerRadius = 8;
};

} // namespace sgui
