#include "column.hpp"
#include <algorithm>

namespace sgui {

std::shared_ptr<Column> Column::create(
    Modifier mod,
    Arrangement verticalArrangement,
    Alignment horizontalAlignment,
    std::initializer_list<std::shared_ptr<Widget>> children
) {
    auto col = std::make_shared<Column>();
    col->modifier = mod;
    col->setVerticalArrangement(verticalArrangement);
    col->setHorizontalAlignment(horizontalAlignment);
    for (const auto& child : children) {
        col->addChild(child);
    }
    return col;
}

Column& Column::addChild(std::shared_ptr<Widget> child) {
    if (child) {
        children_.push_back(std::move(child));
    }
    return *this;
}

Column& Column::setSpacing(int spacing) {
    spacing_ = spacing;
    return *this;
}

Column& Column::setVerticalArrangement(Arrangement arr) {
    verticalArrangement_ = arr;
    return *this;
}

Column& Column::setHorizontalAlignment(Alignment align) {
    horizontalAlignment_ = align;
    return *this;
}

void Column::render(RenderContext& ctx) {
    if (children_.empty()) {
        return;
    }

    Size rawSize = measure();
    // 显式尺寸 > fillMax > 测量值
    int finalWidth = modifier.width_ > 0 ? modifier.width_ : (modifier.fillMaxWidth_ ? ctx.maxWidth : rawSize.width);
    int finalHeight = modifier.height_ > 0 ? modifier.height_ : (modifier.fillMaxHeight_ ? ctx.maxHeight : rawSize.height);

    // 背景
    if (modifier.backgroundColor_.a > 0) {
        DrawRectangle(ctx.x, ctx.y, finalWidth, finalHeight, modifier.backgroundColor_);
    }
    // 边框
    if (modifier.borderWidth_ > 0 && modifier.borderColor_.a > 0) {
        DrawRectangleLinesEx({
            (float)ctx.x,
            (float)ctx.y,
            (float)finalWidth,
            (float)finalHeight
        }, (float)modifier.borderWidth_, modifier.borderColor_);
    }

    // 内容区域（扣除 padding）
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

    int freeSpace = cH - rawSize.height;
    if (freeSpace < 0) {
        freeSpace = 0;
    }

    int startY = cY;
    int gap = spacing_;

    // 主轴（垂直）处理
    switch (verticalArrangement_) {
    case Arrangement::Start:
        break;
    case Arrangement::Center:
        startY += freeSpace / 2;
        break;
    case Arrangement::End:
        startY += freeSpace;
        break;
    case Arrangement::SpaceBetween:
        if (children_.size() > 1) {
            gap += freeSpace / (children_.size() - 1);
        }
        break;
    case Arrangement::SpaceAround:
        if (!children_.empty()) {
            int totalGaps = children_.size() * 2;
            int spacePerGap = freeSpace / totalGaps;
            startY += spacePerGap;
            gap += spacePerGap * 2;
        }
        break;
    case Arrangement::SpaceEvenly:
        if (!children_.empty()) {
            int expectedGaps = children_.size() + 1;
            gap += freeSpace / expectedGaps;
            startY += freeSpace / expectedGaps;
        }
        break;
    }

    int currentY = startY;
    for (auto& child : children_) {
        Size childSize = child->measure();
        int cChildW = child->isFillMaxWidth() ? cW : childSize.width;

        RenderContext childCtx = ctx;
        childCtx.y = currentY;

        // 交叉轴（水平）处理
        switch (horizontalAlignment_) {
        case Alignment::Start:
            childCtx.x = cX;
            break;
        case Alignment::Center:
            childCtx.x = cX + (cW - cChildW) / 2;
            break;
        case Alignment::End:
            childCtx.x = cX + (cW - cChildW);
            break;
        }

        childCtx.maxWidth = cW - (childCtx.x - cX);
        childCtx.maxHeight = cH - (childCtx.y - cY);

        child->render(childCtx);

        currentY += childSize.height + gap;
    }
}

Size Column::measure() const {
    Size size{0, 0};
    bool first = true;
    for (const auto& child : children_) {
        Size childSize = child->measure();
        if (!first) {
            size.height += spacing_;
        }
        size.height += childSize.height;
        size.width = std::max(size.width, childSize.width);
        first = false;
    }
    return size;
}

} // namespace sgui
