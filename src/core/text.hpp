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
#include <optional>

namespace sgui {

class Text : public Widget {
public:
    Text(const char* text);
    Text(const std::string& text);
    ~Text() override;
    
    void render(RenderContext& ctx) override;
    Size measure() const override;

    Text& setFontSize(int size);
    Text& setColor(Color color);
    Text& setFont(Font font);
    Text& loadFont(const std::string& fontPath); // 组件内部自动解析码位并加载字体
    Text& setSpacing(float spacing);

private:
    std::string text_;
    int fontSize_ = 20;
    Color color_ = BLACK;
    float spacing_ = 1.0f;
    std::optional<Font> font_;
    bool ownsFont_ = false; // 标记是否由组件自己管理字体的生命周期
};

} // namespace sgui