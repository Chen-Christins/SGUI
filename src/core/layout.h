#pragma once

#include "widget.h"
#include <memory>
#include <vector>

namespace sgui {

// Overrides parent Alignment for a specific child in Row/Column.
// Detected by Row/Column via dynamic_cast during layout.
class AlignModifier : public Widget {
public:
    AlignModifier(Alignment alignment, std::shared_ptr<Widget> child);

    Alignment alignment() const;

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    Alignment alignment_;
    std::shared_ptr<Widget> child_;
};

// Overlays children on top of each other at the same origin
class Box : public Widget {
public:
    explicit Box(std::vector<std::shared_ptr<Widget>> children);
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::vector<std::shared_ptr<Widget>> children_;
};

// Fills available space in the parent's layout direction
class Spacer : public Widget {
public:
    void render(RenderContext& ctx) override;
    Size measure() const override;
};

} // namespace sgui
