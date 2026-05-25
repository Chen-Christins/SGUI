#pragma once

#include "widget.h"
#include <memory>

namespace sgui {

class Badge : public Widget {
public:
    Badge(int count, std::shared_ptr<Widget> child);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    int count_;
    std::shared_ptr<Widget> child_;

    static constexpr int kBadgeRadius = 10;
    static constexpr int kBadgeDiam = 20;
};

} // namespace sgui
