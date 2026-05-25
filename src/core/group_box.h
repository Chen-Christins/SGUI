#pragma once

#include "widget.h"
#include <memory>
#include <string>

namespace sgui {

class GroupBox : public Widget {
public:
    GroupBox(std::string title, std::shared_ptr<Widget> content);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::string title_;
    std::shared_ptr<Widget> content_;

    static constexpr int kPadding = 12;
    static constexpr int kTitleAreaHeight = 12;
    static constexpr int kCornerRadius = 6;
};

} // namespace sgui
