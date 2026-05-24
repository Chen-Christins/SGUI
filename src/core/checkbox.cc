#include "checkbox.h"
#include <raylib.h>

namespace sgui {

static constexpr int kBoxSize = 20;
static constexpr int kGap = 8;

Checkbox::Checkbox(MutableState<bool>& checkedState, std::string label)
    : checkedState_(checkedState), label_(std::move(label))
{
}

Size Checkbox::measure() const
{
    int textW = MeasureText(label_.c_str(), 20);
    return { kBoxSize + kGap + textW, kBoxSize + 4 };
}

void Checkbox::render(RenderContext& ctx)
{
    bool checked = checkedState_.get();

    Rectangle box = {
        (float)ctx.x + 2, (float)ctx.y + 2,
        (float)kBoxSize, (float)kBoxSize
    };

    // Click detection on the box area
    Vector2 mp = GetMousePosition();
    if (CheckCollisionPointRec(mp, box) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        checkedState_.set(!checked);
    }

    // Draw checkbox background
    DrawRectangleRec(box, checked ? BLUE : LIGHTGRAY);

    // Draw border
    DrawRectangleLinesEx(box, 2.0f, checked ? DARKBLUE : DARKGRAY);

    // Draw check mark
    if (checked)
    {
        Vector2 a = { box.x + 3, box.y + 10 };
        Vector2 b = { box.x + 8, box.y + 16 };
        Vector2 c = { box.x + 18, box.y + 4 };
        DrawLineEx(a, b, 3.0f, WHITE);
        DrawLineEx(b, c, 3.0f, WHITE);
    }

    // Draw label
    DrawText(label_.c_str(), ctx.x + kBoxSize + kGap + 2, ctx.y + 2, 20, BLACK);

    ctx.x += kBoxSize + kGap + MeasureText(label_.c_str(), 20) + 2;
    ctx.y += kBoxSize + 4;
}

} // namespace sgui
