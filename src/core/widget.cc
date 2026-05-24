#include "widget.h"

namespace sgui {

Text::Text(std::string text) 
    : text_(std::move(text)) {
}

void Text::render(RenderContext& ctx) {
    DrawText(text_.c_str(), ctx.x, ctx.y, 20, DARKGRAY);
    ctx.y += 30;
}

Button::Button(std::string text, std::function<void()> onClick) 
    : text_(std::move(text))
    , onClick_(std::move(onClick)) {
}

void Button::render(RenderContext& ctx) {
    int padding = 10;
    int width = MeasureText(text_.c_str(), 20) + padding * 2;
    int height = 40;
    Rectangle bounds = {
        (float)ctx.x,
        (float)ctx.y,
        (float)width,
        (float)height
    };

    Vector2 mousePoint = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePoint, bounds);

    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (onClick_) {
            onClick_();
        }
    }

    DrawRectangleRec(bounds, isHovered ? LIGHTGRAY : GRAY);
    DrawText(text_.c_str(), ctx.x + padding, ctx.y + 10, 20, BLACK);

    ctx.y += height + 10;
}

Column::Column(std::vector<std::shared_ptr<Widget>> children) 
    : children_(std::move(children)) {
}

void Column::render(RenderContext& ctx) {
    for (const auto& child : children_) {
        child->render(ctx);
    }
}

} // namespace sgui
