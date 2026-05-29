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
    // 显式尺寸 > fillMax > 测量值
    int finalWidth = modifier.width_ > 0 ? modifier.width_ :
                     (modifier.fillMaxWidth_ ? ctx.maxWidth : rawSize.width);
    int finalHeight = modifier.height_ > 0 ? modifier.height_ :
                      (modifier.fillMaxHeight_ ? ctx.maxHeight : rawSize.height);

    // 背景
    if (modifier.backgroundColor_.a > 0) {
        DrawRectangle(ctx.x, ctx.y, finalWidth, finalHeight, modifier.backgroundColor_);
    }
    // 边框
    if (modifier.borderWidth_ > 0 && modifier.borderColor_.a > 0) {
        DrawRectangleLinesEx(
            {(float)ctx.x, (float)ctx.y, (float)finalWidth, (float)finalHeight},
            (float)modifier.borderWidth_, modifier.borderColor_);
    }

    // 内容区域（扣除 padding）
    int cX = ctx.x + modifier.paddingLeft_;
    int cY = ctx.y + modifier.paddingTop_;
    int cW = finalWidth - modifier.paddingLeft_ - modifier.paddingRight_;
    int cH = finalHeight - modifier.paddingTop_ - modifier.paddingBottom_;
    if (cW < 0) cW = 0;
    if (cH < 0) cH = 0;

    for (auto& child : children_) {
        Size childSize = child->measure();
        int cChildW = child->isFillMaxWidth() ? cW : childSize.width;
        int cChildH = child->isFillMaxHeight() ? cH : childSize.height;

        RenderContext childCtx = ctx;

        // 根据 2D 对齐方式计算子组件在内容区域内的起始位置
        switch (contentAlignment_) {
            case Alignment2D::TopStart:
                childCtx.x = cX;
                childCtx.y = cY;
                break;
            case Alignment2D::TopCenter:
                childCtx.x = cX + (cW - cChildW) / 2;
                childCtx.y = cY;
                break;
            case Alignment2D::TopEnd:
                childCtx.x = cX + (cW - cChildW);
                childCtx.y = cY;
                break;
            case Alignment2D::CenterStart:
                childCtx.x = cX;
                childCtx.y = cY + (cH - cChildH) / 2;
                break;
            case Alignment2D::Center:
                childCtx.x = cX + (cW - cChildW) / 2;
                childCtx.y = cY + (cH - cChildH) / 2;
                break;
            case Alignment2D::CenterEnd:
                childCtx.x = cX + (cW - cChildW);
                childCtx.y = cY + (cH - cChildH) / 2;
                break;
            case Alignment2D::BottomStart:
                childCtx.x = cX;
                childCtx.y = cY + (cH - cChildH);
                break;
            case Alignment2D::BottomCenter:
                childCtx.x = cX + (cW - cChildW) / 2;
                childCtx.y = cY + (cH - cChildH);
                break;
            case Alignment2D::BottomEnd:
                childCtx.x = cX + (cW - cChildW);
                childCtx.y = cY + (cH - cChildH);
                break;
        }

        childCtx.maxWidth = cW - (childCtx.x - cX);
        childCtx.maxHeight = cH - (childCtx.y - cY);

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