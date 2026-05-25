#pragma once

#include "widget.h"
#include <string>
#include <raylib.h>

namespace sgui {

class Chip : public Widget {
public:
    Chip(std::string label, Color backgroundColor);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::string label_;
    Color backgroundColor_;

    static constexpr int kHeight = 26;
    static constexpr int kPaddingX = 12;
};

} // namespace sgui
