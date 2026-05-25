#pragma once

#include "widget.h"
#include <raylib.h>

namespace sgui {

class ImageView : public Widget {
public:
    explicit ImageView(Texture2D texture);
    ImageView(Texture2D texture, int width, int height);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    Texture2D texture_;
    int width_;
    int height_;
};

} // namespace sgui
