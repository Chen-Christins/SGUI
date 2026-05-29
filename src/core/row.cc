#include "row.hpp"
#include <algorithm>

namespace sgui {

std::shared_ptr<Row> Row::create(
    Modifier mod,
    Arrangement horizontalArrangement,
    Alignment verticalAlignment,
    std::initializer_list<std::shared_ptr<Widget>> children
) {
    auto row = std::make_shared<Row>();
    row->modifier = mod;
    row->setHorizontalArrangement(horizontalArrangement);
    row->setVerticalAlignment(verticalAlignment);
    for (const auto& child : children) {
        row->addChild(child);
    }
    return row;
}

Row& Row::addChild(std::shared_ptr<Widget> child) {
    if (child) {
        children_.push_back(std::move(child));
    }
    return *this;
}

Row& Row::setSpacing(int spacing) {
    spacing_ = spacing;
    return *this;
}

Row& Row::setHorizontalArrangement(Arrangement arr) {
    horizontalArrangement_ = arr;
    return *this;
}

Row& Row::setVerticalAlignment(Alignment align) {
    verticalAlignment_ = align;
    return *this;
}

void Row::render(RenderContext& ctx) {
    if (children_.empty()) return;

    Size rawSize = measure();
    int finalWidth = modifier.fillMaxWidth_ ? ctx.maxWidth : rawSize.width;
    int finalHeight = modifier.fillMaxHeight_ ? ctx.maxHeight : rawSize.height;

    int freeSpace = finalWidth - rawSize.width;
    if (freeSpace < 0) freeSpace = 0;

    int startX = ctx.x;
    int gap = spacing_;

    // 主轴（水平）处理
    switch (horizontalArrangement_) {
        case Arrangement::Start: break;
        case Arrangement::Center: startX += freeSpace / 2; break;
        case Arrangement::End: startX += freeSpace; break;
        case Arrangement::SpaceBetween:
            if (children_.size() > 1) {
                gap += freeSpace / (children_.size() - 1);
            }
            break;
        case Arrangement::SpaceAround:
            if (!children_.empty()) {
                int totalGaps = children_.size() * 2;
                int spacePerGap = freeSpace / totalGaps;
                startX += spacePerGap;
                gap += spacePerGap * 2;
            }
            break;
        case Arrangement::SpaceEvenly:
            if (!children_.empty()) {
                int expectedGaps = children_.size() + 1;
                gap += freeSpace / expectedGaps;
                startX += freeSpace / expectedGaps;
            }
            break;
    }

    int currentX = startX;
    for (auto& child : children_) {
        Size childSize = child->measure();
        int cHeight = child->isFillMaxHeight() ? finalHeight : childSize.height;
        
        RenderContext childCtx = ctx;
        childCtx.x = currentX;
        
        // 交叉轴（垂直）处理
        switch (verticalAlignment_) {
            case Alignment::Start: childCtx.y = ctx.y; break;
            case Alignment::Center: childCtx.y = ctx.y + (finalHeight - cHeight) / 2; break;
            case Alignment::End: childCtx.y = ctx.y + (finalHeight - cHeight); break;
        }

        // 重要：更新子组件剩余的可用边界（防止截断和越界外溢）
        childCtx.maxWidth = ctx.maxWidth - (childCtx.x - ctx.x);
        childCtx.maxHeight = ctx.maxHeight - (childCtx.y - ctx.y);
        
        child->render(childCtx);
        
        currentX += childSize.width + gap;
    }
}

Size Row::measure() const {
    Size size{0, 0};
    bool first = true;
    for (const auto& child : children_) {
        Size childSize = child->measure();
        if (!first) {
            size.width += spacing_;
        }
        size.width += childSize.width;
        size.height = std::max(size.height, childSize.height);
        first = false;
    }
    return size;
}

} // namespace sgui
