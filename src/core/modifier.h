#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <raylib.h>
#include "widget.h"

namespace sgui {

// A widget wrapper that carries a layout weight for Row/Column distribution.
// Parent layout uses dynamic_cast to detect and allocate proportional space.
class WeightModifier : public Widget {
public:
    WeightModifier(float weight, std::shared_ptr<Widget> child);

    float weight() const;
    void setAllocatedSize(int size);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    float weight_;
    int allocatedSize_ = 0;
    std::shared_ptr<Widget> child_;
};

class Modifier {
public:
    Modifier() = default;

    Modifier padding(int px) const;
    Modifier size(int width, int height) const;
    Modifier background(Color color, int radius = 0) const;
    Modifier border(int width, Color color) const;
    Modifier clickable(std::function<void()> onClick) const;
    Modifier weight(float weight) const;
    Modifier offset(int dx, int dy) const;
    Modifier fillMaxWidth() const;
    Modifier fillMaxHeight() const;
    Modifier fillMaxSize() const;

    std::shared_ptr<Widget> then(std::shared_ptr<Widget> child) const;

private:
    struct Entry {
        enum Type : uint8_t {
            Padding, Size, Background, Border, Clickable,
            Weight, Offset, FillMaxWidth, FillMaxHeight, FillMaxSize
        };
        Type type;
        int arg1 = 0;
        int arg2 = 0;
        Color color = BLANK;
        std::function<void()> onClick;
        float weightVal = 0.0f;
    };
    std::vector<Entry> entries_;
};

} // namespace sgui
