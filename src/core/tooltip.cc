#include "tooltip.h"
#include <raylib.h>

namespace sgui {

Tooltip::Tooltip(bool& showTooltip, std::shared_ptr<Widget> content)
    : showTooltip_(showTooltip), content_(std::move(content)) {}

Size Tooltip::measure() const { return {0, 0}; }

void Tooltip::render(RenderContext& ctx) {
    if (!showTooltip_) return;

    Vector2 mp = GetMousePosition();
    Size s = content_->measure();
    int pad = 8;
    int boxW = s.width + pad * 2;
    int boxH = s.height + pad * 2;
    int x = (int)mp.x + 12;
    int y = (int)mp.y + 16;

    // Keep tooltip on screen
    int sw = GetScreenWidth();
    if (x + boxW > sw) x = sw - boxW - 8;

    // Background — two-layer
    Rectangle box = {(float)x, (float)y, (float)boxW, (float)boxH};
    DrawRectangleRounded(box, 4.0f, 16, DARKGRAY);
    Rectangle inner = {box.x + 1.0f, box.y + 1.0f, box.width - 2.0f, box.height - 2.0f};
    DrawRectangleRounded(inner, 3.0f, 16, Color{50, 50, 50, 240});

    RenderContext childCtx = {x + pad, y + pad, -1, -1};
    content_->render(childCtx);

    // ctx unchanged (overlay)
}

} // namespace sgui
