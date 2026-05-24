#pragma once

#include "widget.h"
#include <memory>

namespace sgui {

// A vertical scrolling container.
// Renders its child with clipping and translates mouse wheel to scroll offset.
// showScrollbar: set to false to hide the scrollbar indicator.
class VerticalScroll : public Widget {
public:
    VerticalScroll(float& scrollOffset, std::shared_ptr<Widget> child, bool showScrollbar = true);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    float& scrollOffset_;
    std::shared_ptr<Widget> child_;
    bool showScrollbar_;
};

// A horizontal scrolling container.
// Uses shift+wheel or trackpad horizontal swipe to scroll.
// showScrollbar: set to false to hide the scrollbar indicator.
class HorizontalScroll : public Widget {
public:
    HorizontalScroll(float& scrollOffset, std::shared_ptr<Widget> child, bool showScrollbar = true);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    float& scrollOffset_;
    std::shared_ptr<Widget> child_;
    bool showScrollbar_;
};

} // namespace sgui
