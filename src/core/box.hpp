/**
 * @file box.hpp
 * @brief 盒式/堆叠布局组件
 * @author Christins (chen.christins@qq.com)
 * @copyright Apache 2.0
 */
#pragma once

#include "widget.hpp"
#include "alignment.hpp"
#include <vector>
#include <memory>
#include <initializer_list>

namespace sgui {

class Box : public Widget {
public:
    static std::shared_ptr<Box> create(
        Modifier mod = {},
        Alignment2D contentAlignment = Alignment2D::TopStart,
        std::initializer_list<std::shared_ptr<Widget>> children = {}
    );

    Box& addChild(std::shared_ptr<Widget> child);

    // 内容对齐方式 (相当于 contentAlignment)
    Box& setContentAlignment(Alignment2D align);

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::vector<std::shared_ptr<Widget>> children_;
    Alignment2D contentAlignment_ = Alignment2D::TopStart;
};

} // namespace sgui