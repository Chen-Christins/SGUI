#include "text.hpp"

namespace sgui {

Text::Text(const char* text) 
    : Text(std::string(text)) {
}

Text::Text(const std::string& text) 
    : text_(text) {
}

Text::~Text() {
    if (ownsFont_ && font_.has_value()) {
        UnloadFont(font_.value());
    }
}

Text& Text::setFontSize(int size) {
    fontSize_ = size;
    return *this;
}

Text& Text::setColor(Color color) {
    color_ = color;
    return *this;
}

Text& Text::setFont(Font font) {
    if (ownsFont_ && font_.has_value()) {
        UnloadFont(font_.value());
    }
    font_ = font;
    ownsFont_ = false;
    return *this;
}

Text& Text::loadFont(const std::string& fontPath) {
    if (ownsFont_ && font_.has_value()) {
        UnloadFont(font_.value());
    }
    
    int codepointCount = 0;
    int* codepoints = LoadCodepoints(text_.c_str(), &codepointCount);
    
    // 超采样技巧：加载更高分辨率(比如2倍或3倍)的纹理
    int baseSize = fontSize_ > 0 ? fontSize_ : 32;
    int renderSize = baseSize * 3; 

    Font newFont = LoadFontEx(fontPath.c_str(), renderSize, codepoints, codepointCount);
    UnloadCodepoints(codepoints);
    
    if (newFont.texture.id > 0) {
        // 双线性过滤，结合超高分辨率纹理(下采样渲染)可以获得非常丝滑的字体抗锯齿效果
        SetTextureFilter(newFont.texture, TEXTURE_FILTER_BILINEAR);
        font_ = newFont;
        ownsFont_ = true;
    }
    return *this;
}

Text& Text::setSpacing(float spacing) {
    spacing_ = spacing;
    return *this;
}

void Text::render(RenderContext& ctx) {
    if (font_.has_value()) {
        Vector2 pos = { (float)ctx.x, (float)ctx.y };
        DrawTextEx(font_.value(), text_.c_str(), pos, (float)fontSize_, spacing_, color_);
    } else {
        DrawText(text_.c_str(), ctx.x, ctx.y, fontSize_, color_);
    }
}

Size Text::measure() const {
    if (font_.has_value()) {
        Vector2 size = MeasureTextEx(font_.value(), text_.c_str(), (float)fontSize_, spacing_);
        return { (int)size.x, (int)size.y };
    } else {
        int width = MeasureText(text_.c_str(), fontSize_);
        return { width, fontSize_ };
    }
}

} // namespace sgui