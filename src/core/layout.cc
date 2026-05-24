#include "layout.h"

namespace sgui {

Box::Box(std::vector<std::shared_ptr<Widget>> children)
    : children_(std::move(children)) {
}

Size Box::measure() const {
    int maxW = 0;
    int maxH = 0;
    for (const auto& child : children_) {
        Size s = child->measure();
        if (s.width > maxW) {
            maxW = s.width;
        }
        if (s.height > maxH) {
            maxH = s.height;
        }
    }
    return { maxW, maxH };
}

void Box::render(RenderContext& ctx) {
    int startX = ctx.x;
    int startY = ctx.y;
    int maxW = 0;
    int maxH = 0;

    for (const auto& child : children_) {
        Size s = child->measure();
        if (s.width > maxW) {
            maxW = s.width;
        }
        if (s.height > maxH) {
            maxH = s.height;
        }
    }

    for (const auto& child : children_) {
        RenderContext childCtx = ctx;
        childCtx.maxWidth = maxW;
        childCtx.maxHeight = maxH;
        child->render(childCtx);
    }

    ctx.x = startX + maxW;
    ctx.y = startY + maxH;
}

void Spacer::render(RenderContext& ctx) {
    if (ctx.maxWidth > 0) {
        ctx.x += ctx.maxWidth;
    }
    if (ctx.maxHeight > 0) {
        ctx.y += ctx.maxHeight;
    }
}

Size Spacer::measure() const {
    return { 0, 0 };
}

} // namespace sgui
