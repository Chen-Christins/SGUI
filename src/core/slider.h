#pragma once

#include "remember.h"
#include "widget.h"

namespace sgui {

class Slider : public Widget {
public:
    Slider(MutableState<float>& valueState, float min, float max, float step = 0.0f);

    void render(RenderContext& ctx) override;
    Size measure() const override;
    bool wantsMaxWidth() const override;

private:
    MutableState<float>& valueState_;
    float min_;
    float max_;
    float step_;

    static constexpr int kTrackHeight = 6;
    static constexpr int kThumbRadius = 10;
    static constexpr int kNaturalWidth = 200;
    static constexpr int kNaturalHeight = 30;
};

} // namespace sgui
