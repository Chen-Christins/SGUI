/**
 * @file button.hpp
 * @brief 按钮组件 (类似 Jetpack Compose Button)
 * @author Christins (chen.christins@qq.com)
 * @copyright Apache 2.0
 */
#pragma once

#include "widget.hpp"
#include <functional>
#include <memory>

namespace sgui {

class Button : public Widget {
public:
    using OnClick = std::function<void()>;

    static std::shared_ptr<Button> create(
        Modifier mod = {},
        std::shared_ptr<Widget> content = nullptr,
        OnClick onClick = nullptr
    );

    void render(RenderContext& ctx) override;
    Size measure() const override;
    bool onMouseDown(int x, int y) override;

    void setOnClick(OnClick cb) { onClick_ = std::move(cb); }

private:
    bool containsPoint(int x, int y) const;
    void performClick() { if (onClick_) onClick_(); }

private:
    std::shared_ptr<Widget> content_;
    OnClick onClick_;
    Rectangle bounds_{}; // 最近一次 render 的 bounding rect
};

} // namespace sgui
