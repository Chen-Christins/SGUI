#include "radio_button.h"
#include <raylib.h>

namespace sgui {

RadioButton::RadioButton(MutableState<int>& selectedId, int id, std::string label)
    : selectedId_(selectedId), id_(id), label_(std::move(label)) {}

Size RadioButton::measure() const {
    int textW = MeasureText(label_.c_str(), 20);
    return {kCircleSize + kGap + textW, kCircleSize + 4};
}

void RadioButton::render(RenderContext& ctx) {
    bool selected = (selectedId_.get() == id_);

    int cx = ctx.x + kCircleSize / 2;
    int cy = ctx.y + kCircleSize / 2;
    float outerR = (float)kCircleSize / 2.0f;

    // Click detection on the circle area
    Vector2 mp = GetMousePosition();
    if (CheckCollisionPointCircle(mp, {(float)cx, (float)cy}, outerR + 2.0f) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        selectedId_.set(id_);
    }

    // Outer circle — two-layer for smooth anti-aliased edge
    DrawCircle(cx, cy, outerR, DARKGRAY);
    DrawCircle(cx, cy, outerR - 1.5f, WHITE);

    // Inner dot when selected
    if (selected) {
        DrawCircle(cx, cy, (float)kDotRadius, BLUE);
    }

    // Label
    DrawText(label_.c_str(), ctx.x + kCircleSize + kGap, ctx.y + 2, 20, BLACK);

    ctx.x += kCircleSize + kGap + MeasureText(label_.c_str(), 20);
    ctx.y += kCircleSize + 4;
}

} // namespace sgui
