#pragma once

#include "widget.h"

namespace sgui {

class Separator : public Widget {
public:
    // vertical=false: horizontal line (fills width)
    // vertical=true:  vertical line (fills height)
    explicit Separator(bool vertical = false);

    void render(RenderContext& ctx) override;
    Size measure() const override;
    bool wantsMaxWidth() const override;
    bool wantsMaxHeight() const override;

private:
    bool vertical_;
};

} // namespace sgui
