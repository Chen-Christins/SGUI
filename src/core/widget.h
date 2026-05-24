#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <raylib.h>

namespace sgui {

template <typename T>
class CompositionLocal;

// Theme-able text color — override via LocalTextColor.provides(color, child)
extern CompositionLocal<Color> LocalTextColor;

enum class Arrangement {
    Start,
    Center,
    End,
    SpaceBetween,
    SpaceAround,
    SpaceEvenly
};

enum class Alignment {
    Start,
    Center,
    End
};

struct Size {
    int width = 0;
    int height = 0;
};

struct RenderContext {
    int x;
    int y;
    int maxWidth = -1;  // -1 = unbounded
    int maxHeight = -1; // -1 = unbounded
};

// Base interface for all UI elements
class Widget {
public:
    virtual ~Widget() = default;
    virtual void render(RenderContext& ctx) = 0;
    virtual Size measure() const { return {}; }
    // Returns true if this widget wants to expand vertically to fill maxHeight
    virtual bool wantsMaxHeight() const { return false; }
    // Returns true if this widget wants to expand horizontally to fill maxWidth
    virtual bool wantsMaxWidth() const { return false; }
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
    Column(std::vector<std::shared_ptr<Widget>> children, int spacing = 5, Arrangement arrangement = Arrangement::Start,
           Alignment alignment = Alignment::Start);
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::vector<std::shared_ptr<Widget>> children_;
    int spacing_;
    Arrangement arrangement_;
    Alignment alignment_;
};

// Represents a horizontal row layout
class Row : public Widget {
public:
    Row(std::vector<std::shared_ptr<Widget>> children, int spacing = 10, Arrangement arrangement = Arrangement::Start,
        Alignment alignment = Alignment::Start);
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::vector<std::shared_ptr<Widget>> children_;
    int spacing_;
    Arrangement arrangement_;
    Alignment alignment_;
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
