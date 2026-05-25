#include "separator.h"
#include <raylib.h>

namespace sgui {

Separator::Separator(bool vertical) : vertical_(vertical) {}

Size Separator::measure() const { return vertical_ ? Size{1, 0} : Size{0, 1}; }

bool Separator::wantsMaxWidth() const { return !vertical_; }

bool Separator::wantsMaxHeight() const { return vertical_; }

void Separator::render(RenderContext& ctx) {
    if (vertical_) {
        int lineH = ctx.maxHeight > 0 ? ctx.maxHeight : 20;
        DrawLine(ctx.x, ctx.y, ctx.x, ctx.y + lineH, GRAY);
        ctx.x += 1;
        ctx.y += lineH;
    } else {
        int lineW = ctx.maxWidth > 0 ? ctx.maxWidth : 20;
        DrawLine(ctx.x, ctx.y, ctx.x + lineW, ctx.y, GRAY);
        ctx.x += lineW;
        ctx.y += 1;
    }
}

} // namespace sgui
