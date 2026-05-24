#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "raylib.h"

namespace sgui {

struct RenderContext {
    int x;
    int y;
};

// Base interface for all UI elements
class Widget {
public:
    virtual ~Widget() = default;
    virtual void render(RenderContext& ctx) = 0;
};

// Represents a text label
class Text : public Widget {
public:
    Text(std::string text) : text_(std::move(text)) {}
    
    void render(RenderContext& ctx) override {
        DrawText(text_.c_str(), ctx.x, ctx.y, 20, DARKGRAY);
        ctx.y += 30; // Advance Y position for layout
    }

private:
    std::string text_;
};

// Represents a clickable button
class Button : public Widget {
public:
    Button(std::string text, std::function<void()> onClick) 
        : text_(std::move(text)), onClick_(std::move(onClick)) {}
    
    void render(RenderContext& ctx) override {
        int padding = 10;
        int width = MeasureText(text_.c_str(), 20) + padding * 2;
        int height = 40;
        Rectangle bounds = { (float)ctx.x, (float)ctx.y, (float)width, (float)height };

        Vector2 mousePoint = GetMousePosition();
        bool isHovered = CheckCollisionPointRec(mousePoint, bounds);
        
        // Handle click
        if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (onClick_) onClick_();
        }

        // Draw button background
        DrawRectangleRec(bounds, isHovered ? LIGHTGRAY : GRAY);
        // Draw button text
        DrawText(text_.c_str(), ctx.x + padding, ctx.y + 10, 20, BLACK);
        
        ctx.y += height + 10; // Advance Y position with some margin
    }

private:
    std::string text_;
    std::function<void()> onClick_;
};

// Represents a vertical column layout
class Column : public Widget {
public:
    Column(std::vector<std::shared_ptr<Widget>> children) 
        : children_(std::move(children)) {}
    
    void render(RenderContext& ctx) override {
        // Sequentially render children. ctx.y will be accumulated.
        for (const auto& child : children_) {
            child->render(ctx);
        }
    }

private:
    std::vector<std::shared_ptr<Widget>> children_;
};

} // namespace sgui
