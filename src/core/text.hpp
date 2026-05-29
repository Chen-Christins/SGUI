/**
 * @file text.h
 * @brief 文本组件
 * @author Christins (chen.christins@qq.com)
 * @date 2026-05-29
 * @copyright Apache 2.0
 */
#pragma once

#include "widget.hpp"

#include <string>

namespace sgui {

class Text : public Widget {
public:
    Text(const char* text);
    Text(const std::string& text);
    
    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    std::string text_;
};

} // namespace sgui