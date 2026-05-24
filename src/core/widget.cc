#include "widget.h"
#include "layout.h"
#include "modifier.h"
#include "composition_local.h"

namespace sgui {

// Default text color theme — can be overridden via LocalTextColor.provides()
CompositionLocal<Color> LocalTextColor{DARKGRAY};

Text::Text(std::string text)
    : text_(std::move(text)) {
}

Size Text::measure() const {
    return { MeasureText(text_.c_str(), 20), 30 };
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
    return { MeasureText(text_.c_str(), 20) + padding * 2, 40 };
}

void Button::render(RenderContext& ctx) {
    int padding = 10;
    int width = MeasureText(text_.c_str(), 20) + padding * 2;
    int height = 40;
    Rectangle bounds = {
        (float)ctx.x,
        (float)ctx.y,
        (float)width,
        (float)height
    };

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

Column::Column(std::vector<std::shared_ptr<Widget>> children)
    : children_(std::move(children)) {
}

Size Column::measure() const {
    int maxW = 0;
    int totalH = 0;
    int n = children_.size();
    if (n == 0) {
        return { 0, 0 };
    }
    for (const auto& child : children_) {
        Size s = child->measure();
        if (s.width > maxW) {
            maxW = s.width;
        }
        totalH += s.height;
    }
    totalH += 5 * (n - 1); // spacing
    return { maxW, totalH };
}

void Column::render(RenderContext& ctx) {
    int n = children_.size();
    if (n == 0) {
        return;
    }

    // First pass: classify children and compute totals
    int fixedHeight = 0;
    float totalWeight = 0.0f;
    int spacerCount = 0;
    for (const auto& child : children_) {
        if (dynamic_cast<Spacer*>(child.get())) {
            spacerCount++;
        }
        else if (auto* wm = dynamic_cast<WeightModifier*>(child.get())) {
            fixedHeight += wm->measure().height;
            totalWeight += wm->weight();
        }
        else {
            fixedHeight += child->measure().height;
        }
    }
    int totalSpacing = 5 * (n - 1);
    int availableHeight = ctx.maxHeight > 0 ? ctx.maxHeight : fixedHeight + totalSpacing;
    int remaining = availableHeight - fixedHeight - totalSpacing;
    if (remaining < 0) {
        remaining = 0;
    }

    // Distribute remaining space: weighted children first, spacers split the rest
    int spacerHeight = 0;
    if (spacerCount > 0) {
        spacerHeight = remaining / spacerCount;
    }
    float weightRemaining = (float)remaining;

    int startX = ctx.x;
    int startY = ctx.y;
    int maxX = startX;
    for (const auto& child : children_) {
        int currentX = ctx.x;
        RenderContext childCtx = ctx;

        if (dynamic_cast<Spacer*>(child.get())) {
            childCtx.maxHeight = spacerHeight;
        }
        else if (auto* wm = dynamic_cast<WeightModifier*>(child.get())) {
            float share = totalWeight > 0 ? weightRemaining * wm->weight() / totalWeight : 0.0f;
            wm->setAllocatedSize((int)share);
        }
        else {
            childCtx.maxHeight = -1;
        }

        child->render(childCtx);
        if (childCtx.x > maxX) {
            maxX = childCtx.x;
        }
        ctx.x = currentX;
        ctx.y = childCtx.y + 5;
    }
    ctx.x = maxX;
    ctx.y = startY + availableHeight;
}

Row::Row(std::vector<std::shared_ptr<Widget>> children, int spacing)
    : children_(std::move(children)), spacing_(spacing) {
}

Size Row::measure() const {
    int totalW = 0;
    int maxH = 0;
    int n = children_.size();
    if (n == 0) {
        return { 0, 0 };
    }
    for (const auto& child : children_) {
        Size s = child->measure();
        totalW += s.width;
        if (s.height > maxH) {
            maxH = s.height;
        }
    }
    totalW += spacing_ * (n - 1);
    return { totalW, maxH };
}

void Row::render(RenderContext& ctx) {
    int n = children_.size();
    if (n == 0) {
        return;
    }

    // First pass: classify children and compute totals
    int fixedWidth = 0;
    float totalWeight = 0.0f;
    int spacerCount = 0;
    for (const auto& child : children_) {
        if (dynamic_cast<Spacer*>(child.get())) {
            spacerCount++;
        }
        else if (auto* wm = dynamic_cast<WeightModifier*>(child.get())) {
            fixedWidth += wm->measure().width;
            totalWeight += wm->weight();
        }
        else {
            fixedWidth += child->measure().width;
        }
    }
    int totalSpacing = spacing_ * (n - 1);
    int availableWidth = ctx.maxWidth > 0 ? ctx.maxWidth : fixedWidth + totalSpacing;
    int remaining = availableWidth - fixedWidth - totalSpacing;
    if (remaining < 0) {
        remaining = 0;
    }

    // Distribute remaining space: weighted children first, spacers split the rest
    int spacerWidth = 0;
    if (spacerCount > 0) {
        spacerWidth = remaining / spacerCount;
    }
    float weightRemaining = (float)remaining;

    int startX = ctx.x;
    int startY = ctx.y;
    int maxY = startY;
    for (const auto& child : children_) {
        int currentY = ctx.y;
        RenderContext childCtx = ctx;

        if (dynamic_cast<Spacer*>(child.get())) {
            childCtx.maxWidth = spacerWidth;
        }
        else if (auto* wm = dynamic_cast<WeightModifier*>(child.get())) {
            float share = totalWeight > 0 ? weightRemaining * wm->weight() / totalWeight : 0.0f;
            wm->setAllocatedSize((int)share);
        }
        else {
            childCtx.maxWidth = -1;
        }

        child->render(childCtx);
        if (childCtx.y > maxY) {
            maxY = childCtx.y;
        }
        ctx.x = childCtx.x + spacing_;
        ctx.y = currentY;
    }
    ctx.x = startX + availableWidth;
    ctx.y = maxY;
}

Padding::Padding(int padding, std::shared_ptr<Widget> child)
    : padding_(padding), child_(std::move(child)) {
}

Size Padding::measure() const {
    Size s = child_->measure();
    return { s.width + padding_ * 2, s.height + padding_ * 2 };
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
