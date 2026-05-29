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

namespace sgui {

class Column : public Widget {
public:
    Column& addChild(std::shared_ptr<Widget> child);
    Column& setSpacing(int spacing);
    
    // 类似于 Compose 的 modifier
    Column& fillMaxWidth(bool fill = true);
    Column& fillMaxHeight(bool fill = true);

    // 主轴排列规则 (Arrangement.Top / Center / Bottom ...)
    Column& setVerticalArrangement(Arrangement arr);
    // 交叉轴对齐规则 (Alignment.Start / CenterHorizontally ...)
    Column& setHorizontalAlignment(Alignment align);
    
    void render(RenderContext& ctx) override;
    Size measure() const override;

    bool isFillMaxWidth() const override { return fillMaxWidth_; }
    bool isFillMaxHeight() const override { return fillMaxHeight_; }

private:
    std::vector<std::shared_ptr<Widget>> children_;
    int spacing_ = 0;
    bool fillMaxWidth_ = false;
    bool fillMaxHeight_ = false;
    Arrangement verticalArrangement_ = Arrangement::Start;
    Alignment horizontalAlignment_ = Alignment::Start;
};

} // namespace sgui