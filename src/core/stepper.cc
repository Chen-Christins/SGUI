#include "stepper.h"
#include <raylib.h>

namespace sgui {

Stepper::Stepper(MutableState<int>& value, int min, int max, int step)
    : value_(value), min_(min), max_(max), step_(step) {}

Size Stepper::measure() const { return {kWidth, kHeight}; }

void Stepper::render(RenderContext& ctx) {
    int startX = ctx.x;
    int startY = ctx.y;
    int val = value_.get();
    Vector2 mp = GetMousePosition();

    // Minus button
    Rectangle minusRect = {(float)startX, (float)startY, (float)kBtnWidth, (float)kHeight};
    bool hoverMinus = CheckCollisionPointRec(mp, minusRect);

    DrawRectangleRounded(minusRect, 4.0f, 16, GRAY);
    Rectangle minusInner = {minusRect.x + 1.0f, minusRect.y + 1.0f,
                            minusRect.width - 2.0f, minusRect.height - 2.0f};
    DrawRectangleRounded(minusInner, 3.0f, 16, hoverMinus ? LIGHTGRAY : RAYWHITE);
    DrawText("-", startX + 11, startY + 8, 20, DARKGRAY);

    // Plus button
    Rectangle plusRect = {(float)(startX + kWidth - kBtnWidth), (float)startY,
                          (float)kBtnWidth, (float)kHeight};
    bool hoverPlus = CheckCollisionPointRec(mp, plusRect);

    DrawRectangleRounded(plusRect, 4.0f, 16, GRAY);
    Rectangle plusInner = {plusRect.x + 1.0f, plusRect.y + 1.0f,
                           plusRect.width - 2.0f, plusRect.height - 2.0f};
    DrawRectangleRounded(plusInner, 3.0f, 16, hoverPlus ? LIGHTGRAY : RAYWHITE);
    DrawText("+", startX + kWidth - kBtnWidth + 8, startY + 7, 20, DARKGRAY);

    // Value display
    int valX = startX + kBtnWidth;
    int valW = kWidth - kBtnWidth * 2;
    Rectangle valRect = {(float)valX, (float)startY, (float)valW, (float)kHeight};
    DrawRectangleRounded(valRect, 0.0f, 0, LIGHTGRAY);
    std::string valStr = std::to_string(val);
    int tw = MeasureText(valStr.c_str(), 20);
    DrawText(valStr.c_str(), valX + (valW - tw) / 2, startY + 8, 20, DARKGRAY);

    // Click handling
    if (hoverMinus && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int newVal = val - step_;
        if (newVal < min_) newVal = min_;
        value_.set(newVal);
    }
    if (hoverPlus && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int newVal = val + step_;
        if (newVal > max_) newVal = max_;
        value_.set(newVal);
    }

    ctx.x = startX + kWidth;
    ctx.y = startY + kHeight;
}

} // namespace sgui
