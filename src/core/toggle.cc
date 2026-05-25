#include "toggle.h"
#include <raylib.h>

namespace sgui {

Toggle::Toggle(MutableState<bool>& onState) : onState_(onState) {}

Size Toggle::measure() const { return {kWidth, kHeight}; }

void Toggle::render(RenderContext& ctx) {
    bool on = onState_.get();

    // Track — two-layer filled approach for MSAA-smooth border
    // Outer layer: border color at full size
    // Inner layer: fill color inset by 1.5px
    Rectangle track = {(float)ctx.x, (float)ctx.y, (float)kWidth, (float)kHeight};
    float outerRound = (float)kHeight / 2.0f;
    Color borderColor = on ? DARKBLUE : GRAY;
    Color fillColor = on ? BLUE : LIGHTGRAY;

    DrawRectangleRounded(track, outerRound, 24, borderColor);
    Rectangle inner = {track.x + 1.5f, track.y + 1.5f, track.width - 3.0f, track.height - 3.0f};
    float innerRound = inner.height / 2.0f;
    DrawRectangleRounded(inner, innerRound, 24, fillColor);

    // Thumb — two-layer circle for MSAA-smooth outline
    int thumbX = on ? ctx.x + kWidth - kThumbRadius - kPadding : ctx.x + kThumbRadius + kPadding;
    int thumbY = ctx.y + kHeight / 2;
    float borderR = (float)kThumbRadius + 1.5f;
    DrawCircle(thumbX, thumbY, borderR, GRAY);
    DrawCircle(thumbX, thumbY, (float)kThumbRadius, WHITE);

    // Click detection
    Vector2 mp = GetMousePosition();
    Rectangle bounds = {(float)ctx.x, (float)ctx.y, (float)kWidth, (float)kHeight};
    if (CheckCollisionPointRec(mp, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        onState_.set(!on);
    }

    ctx.x += kWidth;
    ctx.y += kHeight;
}

} // namespace sgui
