#pragma once

#include "widget.h"
#include <raylib.h>

namespace sgui {

class LoadingSpinner : public Widget {
public:
    explicit LoadingSpinner(int size = 24);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    int size_;
};

} // namespace sgui
