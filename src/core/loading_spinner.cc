#include "loading_spinner.h"
#include <cmath>
#include <raylib.h>

namespace sgui {

LoadingSpinner::LoadingSpinner(int size) : size_(size) {}

Size LoadingSpinner::measure() const { return {size_, size_}; }

void LoadingSpinner::render(RenderContext& ctx) {
    int cx = ctx.x + size_ / 2;
    int cy = ctx.y + size_ / 2;
    float r = (float)size_ / 2.0f - 2.0f;

    // Track ring
    DrawRing({(float)cx, (float)cy}, r - 3.0f, r, 0.0f, 360.0f, 0, LIGHTGRAY);

    // Animated arc — rotates over time
    double t = GetTime();
    float startAngle = (float)(t * 200.0); // degrees per second
    float arcLength = 90.0f + 30.0f * (float)std::sin(t * 2.0);
    DrawRing({(float)cx, (float)cy}, r - 3.0f, r, startAngle, startAngle + arcLength, 0, BLUE);

    ctx.x += size_;
    ctx.y += size_;
}

} // namespace sgui
