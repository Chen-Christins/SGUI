#pragma once

#include "remember.h"
#include "widget.h"
#include <memory>
#include <string>
#include <vector>

namespace sgui {

// A row of clickable tab labels that track selection via MutableState<int>
class TabBar : public Widget {
public:
    TabBar(MutableState<int>& selectedTab, std::vector<std::string> labels);

    void render(RenderContext& ctx) override;
    Size measure() const override;
    bool wantsMaxWidth() const override;

private:
    MutableState<int>& selectedTab_;
    std::vector<std::string> labels_;

    static constexpr int kTabPaddingX = 16;
    static constexpr int kTabPaddingY = 10;
    static constexpr int kIndicatorHeight = 3;
};

// Only renders its content when index matches selectedTab
class TabPanel : public Widget {
public:
    TabPanel(int index, MutableState<int>& selectedTab, std::shared_ptr<Widget> content);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    int index_;
    MutableState<int>& selectedTab_;
    std::shared_ptr<Widget> content_;
};

} // namespace sgui
