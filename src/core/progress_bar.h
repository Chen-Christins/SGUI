#pragma once

#include "remember.h"
#include "widget.h"

namespace sgui {

class ProgressBar : public Widget {
public:
    // progress: reactive float value, clamped to [0, 1]
    explicit ProgressBar(MutableState<float>& progress);

    void render(RenderContext& ctx) override;
    Size measure() const override;
    bool wantsMaxWidth() const override;

private:
    MutableState<float>& progress_;

    static constexpr int kNaturalWidth = 200;
    static constexpr int kHeight = 20;
    static constexpr int kCornerRadius = 10;
};

} // namespace sgui
