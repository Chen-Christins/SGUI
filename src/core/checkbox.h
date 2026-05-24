#pragma once

#include <string>
#include "widget.h"
#include "remember.h"

namespace sgui {

// A toggleable checkbox with a text label.
// State (checked) is stored in MutableState so it survives recomposition.
class Checkbox : public Widget {
public:
    // checkedState: the boolean state for this checkbox.
    // label: text displayed next to the checkbox.
    Checkbox(MutableState<bool>& checkedState, std::string label);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    MutableState<bool>& checkedState_;
    std::string label_;
};

} // namespace sgui
