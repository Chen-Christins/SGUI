#include "image.h"

namespace sgui {

ImageView::ImageView(Texture2D texture) : texture_(texture), width_(texture.width), height_(texture.height) {}

ImageView::ImageView(Texture2D texture, int width, int height) : texture_(texture), width_(width), height_(height) {}

Size ImageView::measure() const { return {width_, height_}; }

void ImageView::render(RenderContext& ctx) {
    int rw = (ctx.maxWidth > 0) ? ctx.maxWidth : width_;
    int rh = (ctx.maxHeight > 0) ? ctx.maxHeight : height_;

    // Maintain aspect ratio if only one axis is constrained
    if (ctx.maxWidth > 0 && ctx.maxHeight <= 0) {
        float scale = (float)ctx.maxWidth / (float)width_;
        rh = (int)(height_ * scale);
    } else if (ctx.maxHeight > 0 && ctx.maxWidth <= 0) {
        float scale = (float)ctx.maxHeight / (float)height_;
        rw = (int)(width_ * scale);
    }

    Rectangle src = {0.0f, 0.0f, (float)texture_.width, (float)texture_.height};
    Rectangle dst = {(float)ctx.x, (float)ctx.y, (float)rw, (float)rh};
    Vector2 origin = {0.0f, 0.0f};
    DrawTexturePro(texture_, src, dst, origin, 0.0f, WHITE);

    ctx.x += rw;
    ctx.y += rh;
}

} // namespace sgui
