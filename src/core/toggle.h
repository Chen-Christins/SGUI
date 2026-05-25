#pragma once

#include "remember.h"
#include "widget.h"

namespace sgui {

class Toggle : public Widget {
public:
    explicit Toggle(MutableState<bool>& onState);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    MutableState<bool>& onState_;

    static constexpr int kWidth = 44;
    static constexpr int kHeight = 24;
    static constexpr int kThumbRadius = 10;
    static constexpr int kPadding = 2;
};

} // namespace sgui
