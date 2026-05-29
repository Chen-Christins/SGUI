#include "box.hpp"
#include <algorithm>

namespace sgui {

std::shared_ptr<Box> Box::create(
    Modifier mod,
    Alignment2D contentAlignment,
    std::initializer_list<std::shared_ptr<Widget>> children
) {
    auto box = std::make_shared<Box>();
    box->modifier = mod;
    box->setContentAlignment(contentAlignment);
    for (const auto& child : children) {
        box->addChild(child);
    }
    return box;
}

Box& Box::addChild(std::shared_ptr<Widget> child) {
    if (child) {
        children_.push_back(std::move(child));
    }
    return *this;
}

Box& Box::setContentAlignment(Alignment2D align) {
    contentAlignment_ = align;
    return *this;
}

void Box::render(RenderContext& ctx) {
    Size rawSize = measure();
    int finalWidth = modifier.fillMaxWidth_ ? ctx.maxWidth : rawSize.width;
    int finalHeight = modifier.fillMaxHeight_ ? ctx.maxHeight : rawSize.height;

    if (modifier.backgroundColor_.a > 0) {
        DrawRectangle(ctx.x, ctx.y, finalWidth, finalHeight, modifier.backgroundColor_);
    }

    for (auto& child : children_) {
        Size childSize = child->measure();
        int cWidth = child->isFillMaxWidth() ? finalWidth : childSize.width;
        int cHeight = child->isFillMaxHeight() ? finalHeight : childSize.height;
        
        RenderContext childCtx = ctx;

        // 根据 2D 对齐方式计算子组件的起始位置
        switch (contentAlignment_) {
            case Alignment2D::TopStart:
                childCtx.x = ctx.x;
                childCtx.y = ctx.y;
                break;
            case Alignment2D::TopCenter:
                childCtx.x = ctx.x + (finalWidth - cWidth) / 2;
                childCtx.y = ctx.y;
                break;
            case Alignment2D::TopEnd:
                childCtx.x = ctx.x + (finalWidth - cWidth);
                childCtx.y = ctx.y;
                break;
            case Alignment2D::CenterStart:
                childCtx.x = ctx.x;
                childCtx.y = ctx.y + (finalHeight - cHeight) / 2;
                break;
            case Alignment2D::Center:
                childCtx.x = ctx.x + (finalWidth - cWidth) / 2;
                childCtx.y = ctx.y + (finalHeight - cHeight) / 2;
                break;
            case Alignment2D::CenterEnd:
                childCtx.x = ctx.x + (finalWidth - cWidth);
                childCtx.y = ctx.y + (finalHeight - cHeight) / 2;
                break;
            case Alignment2D::BottomStart:
                childCtx.x = ctx.x;
                childCtx.y = ctx.y + (finalHeight - cHeight);
                break;
            case Alignment2D::BottomCenter:
                childCtx.x = ctx.x + (finalWidth - cWidth) / 2;
                childCtx.y = ctx.y + (finalHeight - cHeight);
                break;
            case Alignment2D::BottomEnd:
                childCtx.x = ctx.x + (finalWidth - cWidth);
                childCtx.y = ctx.y + (finalHeight - cHeight);
                break;
        }
        
        childCtx.maxWidth = ctx.maxWidth - (childCtx.x - ctx.x);
        childCtx.maxHeight = ctx.maxHeight - (childCtx.y - ctx.y);

        child->render(childCtx);
    }
}

Size Box::measure() const {
    Size size{0, 0};
    for (const auto& child : children_) {
        Size childSize = child->measure();
        size.width = std::max(size.width, childSize.width);
        size.height = std::max(size.height, childSize.height);
    }
    return size;
}

} // namespace sgui