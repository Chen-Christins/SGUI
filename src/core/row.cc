#include "row.hpp"
#include <algorithm>

namespace sgui {

std::shared_ptr<Row> Row::create(Modifier mod, Arrangement horizontalArrangement, Alignment verticalAlignment,
                                 std::initializer_list<std::shared_ptr<Widget>> children) {
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
    if (children_.empty()) {
        return;
    }

    Size rawSize = measure();
    // 显式尺寸 > fillMax > 测量值
    int finalWidth = modifier.width_ > 0 ? modifier.width_ : (modifier.fillMaxWidth_ ? ctx.maxWidth : rawSize.width);
    int finalHeight = modifier.height_ > 0 ? modifier.height_ : (modifier.fillMaxHeight_ ? ctx.maxHeight : rawSize.height);

    bounds_ = {(float)ctx.x, (float)ctx.y, (float)finalWidth, (float)finalHeight};

    // 背景
    if (modifier.backgroundColor_.a > 0) {
        if (modifier.roundedCorner_ > 0) {
            DrawRectangleRounded({(float)ctx.x, (float)ctx.y, (float)finalWidth, (float)finalHeight}, modifier.roundedCorner_, 8,
                                 modifier.backgroundColor_);
        } else {
            DrawRectangle(ctx.x, ctx.y, finalWidth, finalHeight, modifier.backgroundColor_);
        }
    }
    // 边框
    if (modifier.borderWidth_ > 0 && modifier.borderColor_.a > 0) {
        if (modifier.roundedCorner_ > 0) {
            DrawRectangleRoundedLines({(float)ctx.x, (float)ctx.y, (float)finalWidth, (float)finalHeight}, modifier.roundedCorner_, 8,
                                      (float)modifier.borderWidth_, modifier.borderColor_);
        } else {
            DrawRectangleLinesEx({(float)ctx.x, (float)ctx.y, (float)finalWidth, (float)finalHeight}, (float)modifier.borderWidth_,
                                 modifier.borderColor_);
        }
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

    int freeSpace = cW - rawSize.width;
    if (freeSpace < 0) {
        freeSpace = 0;
    }

    int startX = cX;
    int gap = spacing_;

    // 主轴（水平）处理
    switch (horizontalArrangement_) {
    case Arrangement::Start:
        break;
    case Arrangement::Center:
        startX += freeSpace / 2;
        break;
    case Arrangement::End:
        startX += freeSpace;
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
        int cChildH = child->isFillMaxHeight() ? cH : childSize.height;

        RenderContext childCtx = ctx;
        childCtx.x = currentX;

        // 交叉轴（垂直）处理
        switch (verticalAlignment_) {
        case Alignment::Start:
            childCtx.y = cY;
            break;
        case Alignment::Center:
            childCtx.y = cY + (cH - cChildH) / 2;
            break;
        case Alignment::End:
            childCtx.y = cY + (cH - cChildH);
            break;
        }

        childCtx.maxWidth = cW - (childCtx.x - cX);
        childCtx.maxHeight = cH - (childCtx.y - cY);

        child->render(childCtx);

        int childW = child->isFillMaxWidth() ? cW : childSize.width;
        currentX += childW + gap;
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
    if (modifier.width_ > 0) {
        size.width = modifier.width_;
    }
    if (modifier.height_ > 0) {
        size.height = modifier.height_;
    }
    return size;
}

bool Row::onMouseDown(int x, int y) {
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if ((*it)->onMouseDown(x, y)) {
            return true;
        }
    }
    if (modifier.onClick_ && hitTestRoundedRect(x, y, bounds_, modifier.roundedCorner_)) {
        modifier.onClick_();
        return true;
    }
    return false;
}

} // namespace sgui
