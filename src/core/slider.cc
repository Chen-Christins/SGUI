#include "slider.h"
#include <algorithm>
#include <cmath>
#include <raylib.h>

namespace sgui {

Slider::Slider(MutableState<float>& valueState, float min, float max, float step)
    : valueState_(valueState), min_(min), max_(max), step_(step) {}

Size Slider::measure() const { return {kNaturalWidth, kNaturalHeight}; }

bool Slider::wantsMaxWidth() const { return true; }

void Slider::render(RenderContext& ctx) {
    int trackWidth = ctx.maxWidth > 0 ? ctx.maxWidth : kNaturalWidth;
    int trackX = ctx.x + kThumbRadius;
    int trackY = ctx.y + kNaturalHeight / 2 - kTrackHeight / 2;
    int innerTrackW = trackWidth - 2 * kThumbRadius;

    float value = valueState_.get();
    float clamped = std::max(min_, std::min(max_, value));
    float ratio = (clamped - min_) / (max_ - min_);

    // Track background — two-layer filled for smooth border
    Rectangle trackRect = {(float)trackX, (float)trackY, (float)innerTrackW, (float)kTrackHeight};
    DrawRectangleRounded(trackRect, 3.0f, 16, GRAY);
    Rectangle innerTrack = {trackRect.x + 1.0f, trackRect.y + 1.0f,
                            trackRect.width - 2.0f, trackRect.height - 2.0f};
    DrawRectangleRounded(innerTrack, 2.0f, 16, LIGHTGRAY);

    // Filled portion — two-layer filled for smooth edge
    if (ratio > 0.0f) {
        float fillW = (float)innerTrackW * ratio;
        if (fillW > 2.0f) {
            Rectangle fillRect = {(float)trackX, (float)trackY, fillW, (float)kTrackHeight};
            DrawRectangleRounded(fillRect, 3.0f, 16, DARKBLUE);
            Rectangle innerFill = {fillRect.x + 1.0f, fillRect.y + 1.0f,
                                   fillRect.width - 2.0f, fillRect.height - 2.0f};
            if (innerFill.width > 0.0f) {
                DrawRectangleRounded(innerFill, 2.0f, 16, BLUE);
            }
        }
    }

    // Thumb — two-layer circle
    int thumbX = trackX + (int)(ratio * (float)innerTrackW);
    int thumbY = ctx.y + kNaturalHeight / 2;
    float borderR = (float)kThumbRadius + 1.5f;
    DrawCircle(thumbX, thumbY, borderR, GRAY);
    DrawCircle(thumbX, thumbY, (float)kThumbRadius, DARKGRAY);

    // Drag interaction
    Vector2 mp = GetMousePosition();
    Rectangle hitRect = {(float)ctx.x, (float)ctx.y, (float)trackWidth, (float)kNaturalHeight};
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mp, hitRect)) {
        float rawRatio = ((float)mp.x - (float)trackX) / (float)innerTrackW;
        if (rawRatio < 0.0f) rawRatio = 0.0f;
        if (rawRatio > 1.0f) rawRatio = 1.0f;
        float newValue = min_ + rawRatio * (max_ - min_);
        if (step_ > 0.0f) {
            newValue = std::roundf((newValue - min_) / step_) * step_ + min_;
            if (newValue < min_) newValue = min_;
            if (newValue > max_) newValue = max_;
        }
        valueState_.set(newValue);
    }

    ctx.x += trackWidth;
    ctx.y += kNaturalHeight;
}

} // namespace sgui
