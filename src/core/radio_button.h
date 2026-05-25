#pragma once

#include "remember.h"
#include "widget.h"
#include <string>

namespace sgui {

class RadioButton : public Widget {
public:
    // selectedId: shared MutableState across all radio buttons in a group
    // id: unique identifier for this radio button within the group
    // label: text displayed next to the radio button
    RadioButton(MutableState<int>& selectedId, int id, std::string label);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    MutableState<int>& selectedId_;
    int id_;
    std::string label_;

    static constexpr int kCircleSize = 20;
    static constexpr int kGap = 8;
    static constexpr int kDotRadius = 5;
};

} // namespace sgui
