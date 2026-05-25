#include "badge.h"
#include <raylib.h>

namespace sgui {

Badge::Badge(int count, std::shared_ptr<Widget> child)
    : count_(count), child_(std::move(child)) {}

Size Badge::measure() const { return child_->measure(); }

void Badge::render(RenderContext& ctx) {
    int startX = ctx.x;
    int startY = ctx.y;

    child_->render(ctx);

    if (count_ <= 0) return;

    int badgeCX = ctx.x - kBadgeRadius + 4;
    int badgeCY = startY + kBadgeRadius - 4;

    // Two-layer circle
    DrawCircle(badgeCX, badgeCY, (float)kBadgeRadius + 1.5f, Color{180, 0, 0, 255});
    DrawCircle(badgeCX, badgeCY, (float)kBadgeRadius, RED);

    // Count text
    std::string text = count_ > 99 ? "99+" : std::to_string(count_);
    int fontSize = count_ > 9 ? 12 : 14;
    int tw = MeasureText(text.c_str(), fontSize);
    DrawText(text.c_str(), badgeCX - tw / 2, badgeCY - fontSize / 2 + 1, fontSize, WHITE);
}

} // namespace sgui
