/**
 * @file spacer.hpp
 * @brief 占位组件
 * @author Christins (chen.christins@qq.com)
 * @copyright Apache 2.0
 */
#pragma once

#include "widget.hpp"

namespace sgui {

class Spacer : public Widget {
public:
    Spacer(int width, int height);
    
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    int width_;
    int height_;
};

} // namespace sgui