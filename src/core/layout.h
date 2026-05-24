#pragma once

#include <memory>
#include <vector>
#include "widget.h"

namespace sgui {

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
