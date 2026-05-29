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
    int finalWidth = modifier.fillMaxWidth_ ? ctx.maxWidth : rawSize.width;
    int finalHeight = modifier.fillMaxHeight_ ? ctx.maxHeight : rawSize.height;

    int freeSpace = finalHeight - rawSize.height;
    if (freeSpace < 0) {
        freeSpace = 0;
    }

    int startY = ctx.y;
    int gap = spacing_;

    // 主轴（垂直）处理
    switch (verticalArrangement_) {
        case Arrangement::Start: break;
        case Arrangement::Center: startY += freeSpace / 2; break;
        case Arrangement::End: startY += freeSpace; break;
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
        int cWidth = child->isFillMaxWidth() ? finalWidth : childSize.width;
        
        RenderContext childCtx = ctx;
        childCtx.y = currentY;

        // 交叉轴（水平）处理
        switch (horizontalAlignment_) {
            case Alignment::Start: childCtx.x = ctx.x; break;
            case Alignment::Center: childCtx.x = ctx.x + (finalWidth - cWidth) / 2; break;
            case Alignment::End: childCtx.x = ctx.x + (finalWidth - cWidth); break;
        }

        // 重要：更新子组件剩余的可用边界（防止截断和越界外溢）
        childCtx.maxWidth = ctx.maxWidth - (childCtx.x - ctx.x);
        childCtx.maxHeight = ctx.maxHeight - (childCtx.y - ctx.y);
        
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
