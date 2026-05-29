#include "button.hpp"

namespace sgui {

std::shared_ptr<Button> Button::create(
    Modifier mod,
    std::shared_ptr<Widget> content,
    OnClick onClick
) {
    auto btn = std::make_shared<Button>();
    btn->modifier = mod;
    btn->content_ = std::move(content);
    btn->onClick_ = std::move(onClick);
    return btn;
}

void Button::render(RenderContext& ctx) {
    Size rawSize = measure();
    int finalWidth = modifier.width_ > 0 ? modifier.width_ : (modifier.fillMaxWidth_ ? ctx.maxWidth : rawSize.width);
    int finalHeight = modifier.height_ > 0 ? modifier.height_ : (modifier.fillMaxHeight_ ? ctx.maxHeight : rawSize.height);

    // 背景 (支持圆角)
    if (modifier.backgroundColor_.a > 0) {
        if (modifier.roundedCorner_ > 0) {
            DrawRectangleRounded({(float)ctx.x, (float)ctx.y, (float)finalWidth, (float)finalHeight}, modifier.roundedCorner_, 8,
                                 modifier.backgroundColor_);
        } else {
            DrawRectangle(ctx.x, ctx.y, finalWidth, finalHeight, modifier.backgroundColor_);
        }
    }

    // 边框 (支持圆角)
    if (modifier.borderWidth_ > 0 && modifier.borderColor_.a > 0) {
        if (modifier.roundedCorner_ > 0) {
            DrawRectangleRoundedLines({(float)ctx.x, (float)ctx.y, (float)finalWidth, (float)finalHeight}, modifier.roundedCorner_, 8,
                                      (float)modifier.borderWidth_, modifier.borderColor_);
        } else {
            DrawRectangleLinesEx({(float)ctx.x, (float)ctx.y, (float)finalWidth, (float)finalHeight}, (float)modifier.borderWidth_,
                                 modifier.borderColor_);
        }
    }

    // 保存边界 (点击检测用)
    bounds_ = {(float)ctx.x, (float)ctx.y, (float)finalWidth, (float)finalHeight};

    // 内容区域 (扣除 padding)
    int cX = ctx.x + modifier.paddingLeft_;
    int cY = ctx.y + modifier.paddingTop_;
    int cW = finalWidth - modifier.paddingLeft_ - modifier.paddingRight_;
    int cH = finalHeight - modifier.paddingTop_ - modifier.paddingBottom_;
    if (cW < 0) {
        cW = 0;
    }
    if (cH < 0) {
        cH = 0;
    }

    if (content_) {
        Size childSize = content_->measure();
        int childW = content_->isFillMaxWidth() ? cW : childSize.width;
        int childH = content_->isFillMaxHeight() ? cH : childSize.height;

        RenderContext childCtx = ctx;
        childCtx.x = cX + (cW - childW) / 2;
        childCtx.y = cY + (cH - childH) / 2;
        childCtx.maxWidth = cW - (childCtx.x - cX);
        childCtx.maxHeight = cH - (childCtx.y - cY);

        content_->render(childCtx);
    }
}

Size Button::measure() const {
    Size base = {modifier.paddingLeft_ + modifier.paddingRight_,
                 modifier.paddingTop_ + modifier.paddingBottom_};
    if (content_) {
        Size childSize = content_->measure();
        base.width += childSize.width;
        base.height += childSize.height;
    }
    if (modifier.width_ > 0) {
        base.width = modifier.width_;
    }
    if (modifier.height_ > 0) {
        base.height = modifier.height_;
    }
    return base;
}

bool Button::containsPoint(int x, int y) const {
    return hitTestRoundedRect(x, y, bounds_, modifier.roundedCorner_);
}

bool Button::onMouseDown(int x, int y) {
    if (containsPoint(x, y)) {
        performClick();
        if (modifier.onClick_) {
            modifier.onClick_();
        }
        return true;
    }
    return false;
}

} // namespace sgui
