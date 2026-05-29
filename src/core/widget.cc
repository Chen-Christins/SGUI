#include "widget.h"
#include "composition_local.h"

namespace sgui {

// Default text color theme — can be overridden via LocalTextColor.provides()
CompositionLocal<Color> LocalTextColor{DARKGRAY};

Text::Text(std::string text) 
    : text_(std::move(text)) {
}

Size Text::measure() const {
    return {MeasureText(text_.c_str(), 20), 30}; 
}

void Text::render(RenderContext& ctx) {
    Color c = LocalTextColor.current();
    DrawText(text_.c_str(), ctx.x, ctx.y, 20, c);
    ctx.x += MeasureText(text_.c_str(), 20);
    ctx.y += 30;
}

Button::Button(std::string text, std::function<void()> onClick) 
    : text_(std::move(text))
    , onClick_(std::move(onClick)) {
}

Size Button::measure() const {
    int padding = 10;
    return {MeasureText(text_.c_str(), 20) + padding * 2, 40};
}

void Button::render(RenderContext& ctx) {
    int padding = 10;
    int width = MeasureText(text_.c_str(), 20) + padding * 2;
    int height = 40;
    Rectangle bounds = {(float)ctx.x, (float)ctx.y, (float)width, (float)height};

    Vector2 mousePoint = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePoint, bounds);

    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (onClick_) {
            onClick_();
        }
    }

    DrawRectangleRec(bounds, isHovered ? LIGHTGRAY : GRAY);
    DrawText(text_.c_str(), ctx.x + padding, ctx.y + 10, 20, BLACK);

    ctx.x += width;
    ctx.y += height;
}

Column::Column(std::vector<std::shared_ptr<Widget>> children, int spacing, Arrangement arrangement, Alignment alignment)
    : children_(std::move(children))
    , spacing_(spacing)
    , arrangement_(arrangement)
    , alignment_(alignment) {
}

Size Column::measure() const {
    int maxW = 0;
    int totalH = 0;
    int n = (int)children_.size();
    if (n == 0) {
        return {0, 0};
    }
    for (const auto& child : children_) {
        Size s = child->measure();
        if (s.width > maxW) {
            maxW = s.width;
        }
        totalH += s.height;
    }
    totalH += spacing_ * (n - 1);
    return {maxW, totalH};
}

void Column::render(RenderContext& ctx) {
    int n = (int)children_.size();
    if (n == 0) {
        return;
    }

    // First pass: measure and classify
    struct ChildInfo {
        Size size;
        bool isSpacer = false;
        float weight = 0.0f;
        Alignment align = Alignment::Start;
    };
    std::vector<ChildInfo> info(n);

    int fixedHeight = 0;
    float totalWeight = 0.0f;
    int spacerCount = 0;
    int maxW = 0;

    for (int i = 0; i < n; i++) {
        const auto& child = children_[i];
        info[i].size = child->measure();
        if (info[i].size.width > maxW) {
            maxW = info[i].size.width;
        }

        if (dynamic_cast<Spacer*>(child.get())) {
            info[i].isSpacer = true;
            spacerCount++;
        } else if (auto* wm = dynamic_cast<WeightModifier*>(child.get())) {
            info[i].weight = wm->weight();
            fixedHeight += info[i].size.height;
            totalWeight += wm->weight();
        } else {
            fixedHeight += info[i].size.height;
        }

        // Check per-child alignment override
        if (auto* am = dynamic_cast<AlignModifier*>(child.get())) {
            info[i].align = am->alignment();
        } else {
            info[i].align = alignment_;
        }
    }

    int totalSpacing = spacing_ * (n - 1);
    int availableHeight = ctx.maxHeight > 0 ? ctx.maxHeight : fixedHeight + totalSpacing;
    int remaining = availableHeight - fixedHeight - totalSpacing;
    if (remaining < 0) {
        remaining = 0;
    }

    // Distribute spacers and weights
    int spacerHeight = 0;
    if (spacerCount > 0) {
        spacerHeight = remaining / spacerCount;
    }
    float weightRemaining = (float)remaining;

    // Compute arrangement offsets
    float leadingGap = 0.0f;
    float betweenGap = 0.0f;
    bool hasExpansiveChildren = (spacerCount > 0 || totalWeight > 0);

    if (hasExpansiveChildren) {
        // Only Start/Center/End apply when spacers/weights are present
        switch (arrangement_) {
        case Arrangement::Start:
            leadingGap = 0;
            break;
        case Arrangement::Center:
            leadingGap = (float)remaining / 2.0f;
            break;
        case Arrangement::End:
            leadingGap = (float)remaining;
            break;
        default:
            break;
        }
    } else {
        int gaps = n - 1;
        switch (arrangement_) {
        case Arrangement::Start:
            break;
        case Arrangement::Center:
            leadingGap = (float)remaining / 2.0f;
            break;
        case Arrangement::End:
            leadingGap = (float)remaining;
            break;
        case Arrangement::SpaceBetween:
            if (gaps > 0) {
                betweenGap = (float)remaining / (float)gaps;
            }
            break;
        case Arrangement::SpaceAround:
            if (n > 0) {
                float perChild = (float)remaining / (float)n;
                leadingGap = perChild / 2.0f;
                betweenGap = perChild;
            }
            break;
        case Arrangement::SpaceEvenly:
            if (n > 0) {
                float perGap = (float)remaining / (float)(n + 1);
                leadingGap = perGap;
                betweenGap = perGap;
            }
            break;
        }
    }

    // Second pass: render children
    int startX = ctx.x;
    int startY = ctx.y;
    int y = startY + (int)leadingGap;

    for (int i = 0; i < n; i++) {
        const auto& child = children_[i];

        // Cross-axis alignment (horizontal for column)
        int cx = startX;
        switch (info[i].align) {
        case Alignment::Start:
            break;
        case Alignment::Center:
            cx += (maxW - info[i].size.width) / 2;
            break;
        case Alignment::End:
            cx += maxW - info[i].size.width;
            break;
        }

        RenderContext childCtx = {cx, y, maxW, -1};

        if (info[i].isSpacer) {
            childCtx.maxHeight = spacerHeight;
        } else if (info[i].weight > 0.0f) {
            auto* wm = dynamic_cast<WeightModifier*>(child.get());
            float share = totalWeight > 0.0f ? weightRemaining * info[i].weight / totalWeight : 0.0f;
            wm->setAllocatedSize((int)share);
            wm->setMainAxisHorizontal(false);
            childCtx.maxHeight = (int)share;
        } else if (child->wantsMaxHeight()) {
            int remainingHeight = availableHeight - (y - startY);
            if (remainingHeight < 0) { remainingHeight = 0; }
            childCtx.maxHeight = remainingHeight;
        }

        child->render(childCtx);

        y = childCtx.y + spacing_ + (int)betweenGap;
    }

    ctx.x = startX + maxW;
    ctx.y = startY + availableHeight;
}

Row::Row(std::vector<std::shared_ptr<Widget>> children, int spacing, Arrangement arrangement, Alignment alignment)
    : children_(std::move(children))
    , spacing_(spacing)
    , arrangement_(arrangement)
    , alignment_(alignment) {
}

Size Row::measure() const {
    int totalW = 0;
    int maxH = 0;
    int n = (int)children_.size();
    if (n == 0) {
        return {0, 0};
    }
    for (const auto& child : children_) {
        Size s = child->measure();
        totalW += s.width;
        if (s.height > maxH) {
            maxH = s.height;
        }
    }
    totalW += spacing_ * (n - 1);
    return {totalW, maxH};
}

void Row::render(RenderContext& ctx) {
    int n = (int)children_.size();
    if (n == 0) {
        return;
    }

    // First pass: measure and classify
    struct ChildInfo {
        Size size;
        bool isSpacer = false;
        float weight = 0.0f;
        Alignment align = Alignment::Start;
    };
    std::vector<ChildInfo> info(n);

    int fixedWidth = 0;
    float totalWeight = 0.0f;
    int spacerCount = 0;
    int maxH = 0;

    for (int i = 0; i < n; i++) {
        const auto& child = children_[i];
        info[i].size = child->measure();
        if (info[i].size.height > maxH) {
            maxH = info[i].size.height;
        }

        if (dynamic_cast<Spacer*>(child.get())) {
            info[i].isSpacer = true;
            spacerCount++;
        } else if (auto* wm = dynamic_cast<WeightModifier*>(child.get())) {
            info[i].weight = wm->weight();
            fixedWidth += info[i].size.width;
            totalWeight += wm->weight();
        } else {
            fixedWidth += info[i].size.width;
        }

        if (auto* am = dynamic_cast<AlignModifier*>(child.get())) {
            info[i].align = am->alignment();
        } else {
            info[i].align = alignment_;
        }
    }

    int totalSpacing = spacing_ * (n - 1);
    int availableWidth = ctx.maxWidth > 0 ? ctx.maxWidth : fixedWidth + totalSpacing;
    int remaining = availableWidth - fixedWidth - totalSpacing;
    if (remaining < 0) {
        remaining = 0;
    }

    // Distribute spacers and weights
    int spacerWidth = 0;
    if (spacerCount > 0) {
        spacerWidth = remaining / spacerCount;
    }
    float weightRemaining = (float)remaining;

    // Compute arrangement offsets
    float leadingGap = 0.0f;
    float betweenGap = 0.0f;
    bool hasExpansiveChildren = (spacerCount > 0 || totalWeight > 0);

    if (hasExpansiveChildren) {
        switch (arrangement_) {
        case Arrangement::Start:
            leadingGap = 0;
            break;
        case Arrangement::Center:
            leadingGap = (float)remaining / 2.0f;
            break;
        case Arrangement::End:
            leadingGap = (float)remaining;
            break;
        default:
            break;
        }
    } else {
        int gaps = n - 1;
        switch (arrangement_) {
        case Arrangement::Start:
            break;
        case Arrangement::Center:
            leadingGap = (float)remaining / 2.0f;
            break;
        case Arrangement::End:
            leadingGap = (float)remaining;
            break;
        case Arrangement::SpaceBetween:
            if (gaps > 0) {
                betweenGap = (float)remaining / (float)gaps;
            }
            break;
        case Arrangement::SpaceAround:
            if (n > 0) {
                float perChild = (float)remaining / (float)n;
                leadingGap = perChild / 2.0f;
                betweenGap = perChild;
            }
            break;
        case Arrangement::SpaceEvenly:
            if (n > 0) {
                float perGap = (float)remaining / (float)(n + 1);
                leadingGap = perGap;
                betweenGap = perGap;
            }
            break;
        }
    }

    // Second pass: render children
    int startX = ctx.x;
    int startY = ctx.y;
    int x = startX + (int)leadingGap;

    for (int i = 0; i < n; i++) {
        const auto& child = children_[i];

        // Cross-axis alignment (vertical for row)
        int cy = startY;
        switch (info[i].align) {
        case Alignment::Start:
            break;
        case Alignment::Center:
            cy += (maxH - info[i].size.height) / 2;
            break;
        case Alignment::End:
            cy += maxH - info[i].size.height;
            break;
        }

        RenderContext childCtx = {x, cy, -1, maxH};

        if (info[i].isSpacer) {
            childCtx.maxWidth = spacerWidth;
        } else if (info[i].weight > 0.0f) {
            auto* wm = dynamic_cast<WeightModifier*>(child.get());
            float share = totalWeight > 0.0f ? weightRemaining * info[i].weight / totalWeight : 0.0f;
            wm->setAllocatedSize((int)share);
            childCtx.maxWidth = (int)share;
        } else if (child->wantsMaxWidth()) {
            int remainingWidth = availableWidth - (x - startX);
            if (remainingWidth < 0) { remainingWidth = 0; }
            childCtx.maxWidth = remainingWidth;
        }

        child->render(childCtx);

        x = childCtx.x + spacing_ + (int)betweenGap;
    }

    ctx.x = startX + availableWidth;
    ctx.y = startY + maxH;
}

Padding::Padding(int padding, std::shared_ptr<Widget> child) 
    : padding_(padding)
    , child_(std::move(child)) {
}

Size Padding::measure() const {
    Size s = child_->measure();
    return {s.width + padding_ * 2, s.height + padding_ * 2};
}

void Padding::render(RenderContext& ctx) {
    int startX = ctx.x;
    int startY = ctx.y;
    ctx.x += padding_;
    ctx.y += padding_;
    child_->render(ctx);
    int innerWidth = ctx.x - (startX + padding_);
    int innerHeight = ctx.y - (startY + padding_);
    ctx.x = startX + padding_ * 2 + innerWidth;
    ctx.y = startY + padding_ * 2 + innerHeight;
}

} // namespace sgui
