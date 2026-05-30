/**
 * @file row.hpp
 * @brief 行布局组件
 * @author Christins (chen.christins@qq.com)
 * @date 2026-05-30
 * @copyright Apache 2.0
 */
#pragma once

#include "widget.hpp"
#include "alignment.hpp"
#include <vector>
#include <memory>
#include <initializer_list>

namespace sgui {

class Row : public Widget {
public:
    static std::shared_ptr<Row> create(
        Modifier mod = {},
        Arrangement horizontalArrangement = Arrangement::Start,
        Alignment verticalAlignment = Alignment::Start,
        std::initializer_list<std::shared_ptr<Widget>> children = {}
    );

    Row& addChild(std::shared_ptr<Widget> child);
    Row& setSpacing(int spacing);

    Row& setHorizontalArrangement(Arrangement arr);
    Row& setVerticalAlignment(Alignment align);

    void render(RenderContext& ctx) override;
    Size measure() const override;
    bool onMouseDown(int x, int y) override;

private:
    std::vector<std::shared_ptr<Widget>> children_;
    int spacing_ = 0;
    Arrangement horizontalArrangement_ = Arrangement::Start;
    Alignment verticalAlignment_ = Alignment::Start;
    Rectangle bounds_{}; // 最近一次 render 的 bounding rect
};

} // namespace sgui