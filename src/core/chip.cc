#include "chip.h"
#include <raylib.h>

namespace sgui {

Chip::Chip(std::string label, Color backgroundColor)
    : label_(std::move(label)), backgroundColor_(backgroundColor) {}

Size Chip::measure() const {
    int tw = MeasureText(label_.c_str(), 18);
    return {tw + kPaddingX * 2, kHeight};
}

void Chip::render(RenderContext& ctx) {
    int tw = MeasureText(label_.c_str(), 18);
    int chipW = tw + kPaddingX * 2;

    float roundness = (float)kHeight / 2.0f;
    Rectangle r = {(float)ctx.x, (float)ctx.y, (float)chipW, (float)kHeight};

    // Two-layer pill
    DrawRectangleRounded(r, roundness, 20, GRAY);
    Rectangle inner = {r.x + 1.0f, r.y + 1.0f, r.width - 2.0f, r.height - 2.0f};
    DrawRectangleRounded(inner, roundness - 1.0f, 20, backgroundColor_);

    // Label — pick contrasting text color based on bg brightness
    bool lightBg = (backgroundColor_.r + backgroundColor_.g + backgroundColor_.b) > 400;
    Color textColor = lightBg ? DARKGRAY : WHITE;
    DrawText(label_.c_str(), ctx.x + kPaddingX, ctx.y + 4, 18, textColor);

    ctx.x += chipW;
    ctx.y += kHeight;
}

} // namespace sgui
