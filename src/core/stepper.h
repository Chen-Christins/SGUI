#pragma once

#include "remember.h"
#include "widget.h"

namespace sgui {

class Stepper : public Widget {
public:
    Stepper(MutableState<int>& value, int min, int max, int step = 1);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    MutableState<int>& value_;
    int min_;
    int max_;
    int step_;

    static constexpr int kWidth = 120;
    static constexpr int kHeight = 36;
    static constexpr int kBtnWidth = 30;
};

} // namespace sgui
