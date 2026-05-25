#include "progress_bar.h"
#include <raylib.h>

namespace sgui {

ProgressBar::ProgressBar(MutableState<float>& progress) : progress_(progress) {}

Size ProgressBar::measure() const { return {kNaturalWidth, kHeight}; }

bool ProgressBar::wantsMaxWidth() const { return true; }

void ProgressBar::render(RenderContext& ctx) {
    int barWidth = ctx.maxWidth > 0 ? ctx.maxWidth : kNaturalWidth;

    float progress = progress_.get();
    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    // Track — two-layer filled for smooth border
    Rectangle track = {(float)ctx.x, (float)ctx.y, (float)barWidth, (float)kHeight};
    DrawRectangleRounded(track, (float)kCornerRadius, 16, GRAY);
    Rectangle inner = {track.x + 1.0f, track.y + 1.0f, track.width - 2.0f, track.height - 2.0f};
    DrawRectangleRounded(inner, (float)(kCornerRadius - 1), 16, LIGHTGRAY);

    // Fill portion
    if (progress > 0.0f) {
        float fillW = (float)barWidth * progress;
        if (fillW > 2.0f) {
            Rectangle fill = {(float)ctx.x, (float)ctx.y, fillW, (float)kHeight};
            DrawRectangleRounded(fill, (float)kCornerRadius, 16, DARKBLUE);
            Rectangle fillInner = {fill.x + 1.0f, fill.y + 1.0f, fill.width - 2.0f, fill.height - 2.0f};
            if (fillInner.width > 0.0f) {
                DrawRectangleRounded(fillInner, (float)(kCornerRadius - 1), 16, BLUE);
            }
        }
    }

    ctx.x += barWidth;
    ctx.y += kHeight;
}

} // namespace sgui
