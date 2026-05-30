/**
 * @file column.hpp
 * @brief 列布局组件
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

class Column : public Widget {
public:
    static std::shared_ptr<Column> create(
        Modifier mod = {},
        Arrangement verticalArrangement = Arrangement::Start,
        Alignment horizontalAlignment = Alignment::Start,
        std::initializer_list<std::shared_ptr<Widget>> children = {}
    );

    Column& addChild(std::shared_ptr<Widget> child);
    Column& setSpacing(int spacing);

    Column& setVerticalArrangement(Arrangement arr);
    Column& setHorizontalAlignment(Alignment align);

    void render(RenderContext& ctx) override;
    Size measure() const override;
    bool onMouseDown(int x, int y) override;

private:
    std::vector<std::shared_ptr<Widget>> children_;
    int spacing_ = 0;
    Arrangement verticalArrangement_ = Arrangement::Start;
    Alignment horizontalAlignment_ = Alignment::Start;
    Rectangle bounds_{}; // 最近一次 render 的 bounding rect
};

} // namespace sgui