#include "modifier.h"
#include "layout.h"

namespace sgui {
namespace {

class PaddingModifier : public Widget {
public:
    PaddingModifier(int px, std::shared_ptr<Widget> child) : px_(px), child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        int startX = ctx.x;
        int startY = ctx.y;
        ctx.x += px_;
        ctx.y += px_;
        child_->render(ctx);
        ctx.x = startX + (ctx.x - startX) + px_;
        ctx.y = startY + (ctx.y - startY) + px_;
    }

    Size measure() const override {
        Size s = child_->measure();
        return {s.width + px_ * 2, s.height + px_ * 2};
    }

private:
    int px_;
    std::shared_ptr<Widget> child_;
};

class SizeModifier : public Widget {
public:
    SizeModifier(int width, int height, std::shared_ptr<Widget> child) : width_(width), height_(height), child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        int startX = ctx.x;
        int startY = ctx.y;
        RenderContext childCtx = ctx;
        if (width_ >= 0) {
            childCtx.maxWidth = width_;
        }
        if (height_ >= 0) {
            childCtx.maxHeight = height_;
        }
        child_->render(childCtx);
        ctx.x = width_ >= 0 ? startX + width_ : childCtx.x;
        ctx.y = height_ >= 0 ? startY + height_ : childCtx.y;
    }

    Size measure() const override {
        Size s = child_->measure();
        return { width_ >= 0 ? width_ : s.width, height_ >= 0 ? height_ : s.height };
    }

private:
    int width_;
    int height_;
    std::shared_ptr<Widget> child_;
};

class BackgroundModifier : public Widget {
public:
    BackgroundModifier(Color color, int radius, std::shared_ptr<Widget> child)
        : color_(color), radius_(radius), child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        Size childSize = child_->measure();
        Rectangle r = {(float)ctx.x, (float)ctx.y, (float)childSize.width, (float)childSize.height};
        if (radius_ > 0) {
            DrawRectangleRounded(r, (float)radius_, 16, color_);
        } else {
            DrawRectangleRec(r, color_);
        }
        child_->render(ctx);
    }

    Size measure() const override { return child_->measure(); }

private:
    Color color_;
    int radius_;
    std::shared_ptr<Widget> child_;
};

class BorderModifier : public Widget {
public:
    BorderModifier(int width, Color color, std::shared_ptr<Widget> child)
        : borderWidth_(width), color_(color), child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        Size childSize = child_->measure();
        Rectangle r = {(float)ctx.x, (float)ctx.y, (float)childSize.width, (float)childSize.height};
        child_->render(ctx);
        DrawRectangleLinesEx(r, (float)borderWidth_, color_);
    }

    Size measure() const override { return child_->measure(); }

private:
    int borderWidth_;
    Color color_;
    std::shared_ptr<Widget> child_;
};

class ClickableModifier : public Widget {
public:
    ClickableModifier(std::function<void()> onClick, std::shared_ptr<Widget> child)
        : onClick_(std::move(onClick)), child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        Size childSize = child_->measure();
        Rectangle bounds = {(float)ctx.x, (float)ctx.y, (float)childSize.width, (float)childSize.height};
        Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (onClick_) {
                onClick_();
            }
        }
        child_->render(ctx);
    }

    Size measure() const override { return child_->measure(); }

private:
    std::function<void()> onClick_;
    std::shared_ptr<Widget> child_;
};

class OffsetModifier : public Widget {
public:
    OffsetModifier(int dx, int dy, std::shared_ptr<Widget> child) : dx_(dx), dy_(dy), child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        int startX = ctx.x;
        int startY = ctx.y;
        ctx.x += dx_;
        ctx.y += dy_;
        child_->render(ctx);
        Size s = child_->measure();
        ctx.x = startX + s.width + dx_;
        ctx.y = startY + s.height + dy_;
    }

    Size measure() const override {
        Size s = child_->measure();
        return {s.width + dx_, s.height + dy_};
    }

private:
    int dx_;
    int dy_;
    std::shared_ptr<Widget> child_;
};

class FillMaxWidthModifier : public Widget {
public:
    explicit FillMaxWidthModifier(std::shared_ptr<Widget> child) : child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        if (ctx.maxWidth > 0) {
            RenderContext childCtx = ctx;
            childCtx.maxWidth = ctx.maxWidth;
            child_->render(childCtx);
            ctx.x = childCtx.x;
            ctx.y = childCtx.y;
        } else {
            child_->render(ctx);
        }
    }

    Size measure() const override { return child_->measure(); }
    bool wantsMaxHeight() const override { return child_->wantsMaxHeight(); }
    bool wantsMaxWidth() const override { return true; }

private:
    std::shared_ptr<Widget> child_;
};

class FillMaxHeightModifier : public Widget {
public:
    explicit FillMaxHeightModifier(std::shared_ptr<Widget> child) : child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        if (ctx.maxHeight > 0) {
            RenderContext childCtx = ctx;
            childCtx.maxHeight = ctx.maxHeight;
            child_->render(childCtx);
            ctx.x = childCtx.x;
            ctx.y = childCtx.y;
        } else {
            child_->render(ctx);
        }
    }

    Size measure() const override { return child_->measure(); }
    bool wantsMaxHeight() const override { return true; }
    bool wantsMaxWidth() const override { return child_->wantsMaxWidth(); }

private:
    std::shared_ptr<Widget> child_;
};

class FillMaxSizeModifier : public Widget {
public:
    explicit FillMaxSizeModifier(std::shared_ptr<Widget> child) : child_(std::move(child)) {}

    void render(RenderContext& ctx) override {
        RenderContext childCtx = ctx;
        if (ctx.maxWidth > 0) {
            childCtx.maxWidth = ctx.maxWidth;
        }
        if (ctx.maxHeight > 0) {
            childCtx.maxHeight = ctx.maxHeight;
        }
        child_->render(childCtx);
        ctx.x = childCtx.x;
        ctx.y = childCtx.y;
    }

    Size measure() const override { return child_->measure(); }
    bool wantsMaxHeight() const override { return true; }
    bool wantsMaxWidth() const override { return true; }

private:
    std::shared_ptr<Widget> child_;
};

} // namespace

// ---- WeightModifier (public, used by Row/Column for layout) ----

WeightModifier::WeightModifier(float weight, std::shared_ptr<Widget> child) : weight_(weight), child_(std::move(child)) {}

float WeightModifier::weight() const { return weight_; }

void WeightModifier::setAllocatedSize(int size) { allocatedSize_ = size; }

void WeightModifier::setMainAxisHorizontal(bool horizontal) { mainAxisHorizontal_ = horizontal; }

void WeightModifier::render(RenderContext& ctx) {
    int startX = ctx.x;
    int startY = ctx.y;
    child_->render(ctx);
    if (allocatedSize_ > 0) {
        if (mainAxisHorizontal_) {
            ctx.x = startX + allocatedSize_;
            ctx.y = startY + (ctx.y - startY);
        } else {
            ctx.x = startX + (ctx.x - startX);
            ctx.y = startY + allocatedSize_;
        }
    }
}

Size WeightModifier::measure() const { return child_->measure(); }

// ---- Modifier methods ----

Modifier Modifier::padding(int px) const {
    Modifier m = *this;
    m.entries_.push_back({Entry::Padding, px});
    return m;
}

Modifier Modifier::size(int width, int height) const {
    Modifier m = *this;
    m.entries_.push_back({Entry::Size, width, height});
    return m;
}

Modifier Modifier::background(Color color, int radius) const {
    Modifier m = *this;
    m.entries_.push_back({Entry::Background, radius, 0, color});
    return m;
}

Modifier Modifier::border(int width, Color color) const {
    Modifier m = *this;
    m.entries_.push_back({Entry::Border, width, 0, color});
    return m;
}

Modifier Modifier::clickable(std::function<void()> onClick) const {
    Modifier m = *this;
    m.entries_.push_back({Entry::Clickable});
    m.entries_.back().onClick = std::move(onClick);
    return m;
}

Modifier Modifier::weight(float weight) const {
    Modifier m = *this;
    m.entries_.push_back({Entry::Weight, 0, 0, BLANK, {}, weight});
    return m;
}

Modifier Modifier::offset(int dx, int dy) const {
    Modifier m = *this;
    m.entries_.push_back({Entry::Offset, dx, dy});
    return m;
}

Modifier Modifier::fillMaxWidth() const {
    Modifier m = *this;
    m.entries_.push_back({Entry::FillMaxWidth});
    return m;
}

Modifier Modifier::fillMaxHeight() const {
    Modifier m = *this;
    m.entries_.push_back({Entry::FillMaxHeight});
    return m;
}

Modifier Modifier::fillMaxSize() const {
    Modifier m = *this;
    m.entries_.push_back({Entry::FillMaxSize});
    return m;
}

Modifier Modifier::align(Alignment alignment) const {
    Modifier m = *this;
    m.entries_.push_back({Entry::Align, (int)alignment});
    return m;
}

std::shared_ptr<Widget> Modifier::then(std::shared_ptr<Widget> child) const {
    auto result = child;
    for (auto it = entries_.rbegin(); it != entries_.rend(); ++it) {
        switch (it->type) {
        case Entry::Padding:
            result = std::make_shared<PaddingModifier>(it->arg1, result);
            break;
        case Entry::Size:
            result = std::make_shared<SizeModifier>(it->arg1, it->arg2, result);
            break;
        case Entry::Background:
            result = std::make_shared<BackgroundModifier>(it->color, it->arg1, result);
            break;
        case Entry::Border:
            result = std::make_shared<BorderModifier>(it->arg1, it->color, result);
            break;
        case Entry::Clickable:
            result = std::make_shared<ClickableModifier>(it->onClick, result);
            break;
        case Entry::Offset:
            result = std::make_shared<OffsetModifier>(it->arg1, it->arg2, result);
            break;
        case Entry::FillMaxWidth:
            result = std::make_shared<FillMaxWidthModifier>(result);
            break;
        case Entry::FillMaxHeight:
            result = std::make_shared<FillMaxHeightModifier>(result);
            break;
        case Entry::FillMaxSize:
            result = std::make_shared<FillMaxSizeModifier>(result);
            break;
        case Entry::Align:
            result = std::make_shared<AlignModifier>((Alignment)it->arg1, result);
            break;
        case Entry::Weight:
            result = std::make_shared<WeightModifier>(it->weightVal, result);
            break;
        }
    }
    return result;
}

} // namespace sgui
