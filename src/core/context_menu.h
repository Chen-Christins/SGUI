#pragma once

#include "remember.h"
#include "widget.h"
#include <string>
#include <vector>

namespace sgui {

class ContextMenu : public Widget {
public:
    ContextMenu(MutableState<bool>& isOpen, int& screenX, int& screenY,
                std::vector<std::string> items,
                MutableState<int>& selectedItem);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    MutableState<bool>& isOpen_;
    int& screenX_;
    int& screenY_;
    std::vector<std::string> items_;
    MutableState<int>& selectedItem_;

    static constexpr int kItemHeight = 28;
    static constexpr int kItemPaddingX = 12;
    static constexpr int kMinWidth = 140;
};

} // namespace sgui
