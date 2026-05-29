/**
 * @file box.hpp
 * @brief 盒式/堆叠布局组件 (类似 Jetpack Compose 的 Box)
 * @author Christins (chen.christins@qq.com)
 * @copyright Apache 2.0
 */
#pragma once

#include "widget.hpp"
#include "alignment.hpp"
#include <vector>
#include <memory>

namespace sgui {

class Box : public Widget {
public:
    Box& addChild(std::shared_ptr<Widget> child);
    
    // 类似于 Compose 的 modifier
    Box& fillMaxWidth(bool fill = true);
    Box& fillMaxHeight(bool fill = true);
    
    // 内容对齐方式 (相当于 contentAlignment)
    Box& setContentAlignment(Alignment2D align);

    void render(RenderContext& ctx) override;
    Size measure() const override;

    bool isFillMaxWidth() const override { return fillMaxWidth_; }
    bool isFillMaxHeight() const override { return fillMaxHeight_; }

private:
    std::vector<std::shared_ptr<Widget>> children_;
    bool fillMaxWidth_ = false;
    bool fillMaxHeight_ = false;
    Alignment2D contentAlignment_ = Alignment2D::TopStart;
};

} // namespace sgui