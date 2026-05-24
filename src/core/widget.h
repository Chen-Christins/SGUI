#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <raylib.h>

namespace sgui {

template<typename T> class CompositionLocal;

// Theme-able text color — override via LocalTextColor.provides(color, child)
extern CompositionLocal<Color> LocalTextColor;

struct Size {
    int width = 0;
    int height = 0;
};

struct RenderContext {
    int x;
    int y;
    int maxWidth = -1;   // -1 = unbounded
    int maxHeight = -1;  // -1 = unbounded
};

// Base interface for all UI elements
class Widget {
public:
    virtual ~Widget() = default;
    virtual void render(RenderContext& ctx) = 0;
    virtual Size measure() const { return {}; }
};

// Represents a text label
class Text : public Widget {
public:
    Text(std::string text);
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::string text_;
};

// Represents a clickable button
class Button : public Widget {
public:
    Button(std::string text, std::function<void()> onClick);
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::string text_;
    std::function<void()> onClick_;
};

// Represents a vertical column layout
class Column : public Widget {
public:
    Column(std::vector<std::shared_ptr<Widget>> children);
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::vector<std::shared_ptr<Widget>> children_;
};

// Represents a horizontal row layout
class Row : public Widget {
public:
    Row(std::vector<std::shared_ptr<Widget>> children, int spacing = 10);
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::vector<std::shared_ptr<Widget>> children_;
    int spacing_;
};

// Represents padding around a widget
class Padding : public Widget {
public:
    Padding(int padding, std::shared_ptr<Widget> child);
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    int padding_;
    std::shared_ptr<Widget> child_;
};

} // namespace sgui
