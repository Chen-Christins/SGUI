#include "scroll.h"
#include <algorithm>
#include <raylib.h>

namespace sgui {

static constexpr int kScrollbarThickness = 6;
static constexpr int kScrollSpeed = 30;

// ---- VerticalScroll ----

VerticalScroll::VerticalScroll(float& scrollOffset, std::shared_ptr<Widget> child, bool showScrollbar)
    : scrollOffset_(scrollOffset), child_(std::move(child)), showScrollbar_(showScrollbar) {}

Size VerticalScroll::measure() const { return child_->measure(); }

void VerticalScroll::render(RenderContext& ctx) {
    int vpX = ctx.x;
    int vpY = ctx.y;
    int vpW = ctx.maxWidth > 0 ? ctx.maxWidth : 400;
    int vpH = ctx.maxHeight > 0 ? ctx.maxHeight : 400;

    Size contentSize = child_->measure();

    // Clamp scroll offset
    float maxScroll = (float)std::max(0, contentSize.height - vpH);
    if (scrollOffset_ < 0.0f) {
        scrollOffset_ = 0.0f;
    }
    if (scrollOffset_ > maxScroll) {
        scrollOffset_ = maxScroll;
    }

    // Mouse wheel — only inside viewport
    Vector2 mp = GetMousePosition();
    Rectangle vpRect = {(float)vpX, (float)vpY, (float)vpW, (float)vpH};
    if (CheckCollisionPointRec(mp, vpRect)) {
        float wheel = GetMouseWheelMoveV().y;
        scrollOffset_ -= wheel * kScrollSpeed;
        if (scrollOffset_ < 0.0f) {
            scrollOffset_ = 0.0f;
        }
        if (scrollOffset_ > maxScroll) {
            scrollOffset_ = maxScroll;
        }
    }

    // Clip and render
    BeginScissorMode(vpX, vpY, vpW, vpH);

    int savedX = ctx.x;
    int savedY = ctx.y;
    ctx.y = vpY - (int)scrollOffset_;

    RenderContext childCtx = ctx;
    childCtx.maxWidth = vpW;
    childCtx.maxHeight = -1;
    child_->render(childCtx);

    ctx.x = savedX;
    ctx.y = savedY;
    EndScissorMode();

    // Scrollbar
    if (showScrollbar_ && contentSize.height > vpH) {
        int barX = vpX + vpW - kScrollbarThickness - 2;
        float barH = (float)vpH * (float)vpH / (float)contentSize.height;
        if (barH < 20.0f) {
            barH = 20.0f;
        }
        float barTravel = (float)(vpH - (int)barH);
        float barY = vpY + (scrollOffset_ / maxScroll) * barTravel;
        DrawRectangleRounded({(float)barX, barY, (float)kScrollbarThickness, barH}, 3.0f, 4, Fade(DARKGRAY, 0.5f));
    }

    ctx.x = vpX + vpW;
    ctx.y = vpY + vpH;
}

// ---- HorizontalScroll ----

HorizontalScroll::HorizontalScroll(float& scrollOffset, std::shared_ptr<Widget> child, bool showScrollbar)
    : scrollOffset_(scrollOffset), child_(std::move(child)), showScrollbar_(showScrollbar) {}

Size HorizontalScroll::measure() const { return child_->measure(); }

void HorizontalScroll::render(RenderContext& ctx) {
    int vpX = ctx.x;
    int vpY = ctx.y;
    int vpW = ctx.maxWidth > 0 ? ctx.maxWidth : 400;
    int vpH = ctx.maxHeight > 0 ? ctx.maxHeight : 400;

    Size contentSize = child_->measure();

    // Clamp scroll offset
    float maxScroll = (float)std::max(0, contentSize.width - vpW);
    if (scrollOffset_ < 0.0f) {
        scrollOffset_ = 0.0f;
    }
    if (scrollOffset_ > maxScroll) {
        scrollOffset_ = maxScroll;
    }

    // Mouse wheel — horizontal (shift+wheel or trackpad horizontal swipe)
    Vector2 mp = GetMousePosition();
    Rectangle vpRect = {(float)vpX, (float)vpY, (float)vpW, (float)vpH};
    if (CheckCollisionPointRec(mp, vpRect)) {
        // Use horizontal wheel + shift+vertical wheel for horizontal scroll
        Vector2 wheel = GetMouseWheelMoveV();
        float hw = wheel.x;
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
            hw += wheel.y;
        }
        scrollOffset_ -= hw * kScrollSpeed;
        if (scrollOffset_ < 0.0f) {
            scrollOffset_ = 0.0f;
        }
        if (scrollOffset_ > maxScroll) {
            scrollOffset_ = maxScroll;
        }
    }

    // Clip and render
    BeginScissorMode(vpX, vpY, vpW, vpH);

    int savedX = ctx.x;
    int savedY = ctx.y;
    ctx.x = vpX - (int)scrollOffset_;

    RenderContext childCtx = ctx;
    childCtx.maxWidth = -1;
    childCtx.maxHeight = vpH;
    child_->render(childCtx);

    ctx.x = savedX;
    ctx.y = savedY;
    EndScissorMode();

    // Scrollbar
    if (showScrollbar_ && contentSize.width > vpW) {
        int barY = vpY + vpH - kScrollbarThickness - 2;
        float barW = (float)vpW * (float)vpW / (float)contentSize.width;
        if (barW < 20.0f) {
            barW = 20.0f;
        }
        float barTravel = (float)(vpW - (int)barW);
        float barX = vpX + (scrollOffset_ / maxScroll) * barTravel;
        DrawRectangleRounded({barX, (float)barY, barW, (float)kScrollbarThickness}, 3.0f, 4, Fade(DARKGRAY, 0.5f));
    }

    ctx.x = vpX + vpW;
    ctx.y = vpY + vpH;
}

} // namespace sgui
