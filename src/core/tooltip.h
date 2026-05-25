#pragma once

#include "widget.h"
#include <memory>

namespace sgui {

class Tooltip : public Widget {
public:
    Tooltip(bool& showTooltip, std::shared_ptr<Widget> content);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    bool& showTooltip_;
    std::shared_ptr<Widget> content_;
};

} // namespace sgui
