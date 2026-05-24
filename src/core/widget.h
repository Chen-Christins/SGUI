#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <raylib.h>

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
    Text(std::string text);
    void render(RenderContext& ctx) override;

private:
    std::string text_;
};

// Represents a clickable button
class Button : public Widget {
public:
    Button(std::string text, std::function<void()> onClick);
    void render(RenderContext& ctx) override;

private:
    std::string text_;
    std::function<void()> onClick_;
};

// Represents a vertical column layout
class Column : public Widget {
public:
    Column(std::vector<std::shared_ptr<Widget>> children);
    void render(RenderContext& ctx) override;

private:
    std::vector<std::shared_ptr<Widget>> children_;
};

} // namespace sgui
