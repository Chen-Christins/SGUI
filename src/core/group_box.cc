#include "group_box.h"
#include <raylib.h>

namespace sgui {

GroupBox::GroupBox(std::string title, std::shared_ptr<Widget> content)
    : title_(std::move(title)), content_(std::move(content)) {}

Size GroupBox::measure() const {
    Size s = content_->measure();
    int titleW = MeasureText(title_.c_str(), 20);
    int w = s.width + kPadding * 2;
    if (w < titleW + 20) w = titleW + 20;
    return {w, s.height + kPadding * 2 + kTitleAreaHeight};
}

void GroupBox::render(RenderContext& ctx) {
    int startX = ctx.x;
    int startY = ctx.y;

    int titleW = MeasureText(title_.c_str(), 20);
    Size total = measure();

    // Border rect
    int rectY = startY + (int)(kTitleAreaHeight / 2.0f);
    Rectangle border = {(float)startX, (float)rectY, (float)total.width,
                        (float)(total.height - (int)(kTitleAreaHeight / 2.0f))};
    DrawRectangleRoundedLines(border, (float)kCornerRadius, 16, 2.0f, GRAY);

    // Title background (to cover the border line behind the title)
    int titleX = startX + 14;
    DrawRectangle(titleX - 4, startY, titleW + 8, kTitleAreaHeight + 2, RAYWHITE);

    // Title text
    DrawText(title_.c_str(), titleX, startY, 20, DARKGRAY);

    // Content
    int contentX = startX + kPadding;
    int contentY = rectY + kPadding - kTitleAreaHeight / 2;
    RenderContext childCtx = {contentX, contentY, total.width - kPadding * 2, -1};
    content_->render(childCtx);

    ctx.x = startX + total.width;
    ctx.y = startY + total.height;
}

} // namespace sgui
