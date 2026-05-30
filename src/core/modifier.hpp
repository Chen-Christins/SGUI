/**
 * @file modifier.hpp
 * @brief 组件修饰器 (类似 Jetpack Compose Modifier)
 * @author Christins (chen.christins@qq.com)
 * @copyright Apache 2.0
 */
#pragma once

#include <raylib.h>
#include <functional>

namespace sgui {

class Modifier {
public:
    // —— 尺寸填充 ——
    Modifier& fillMaxWidth(bool fill = true) {
        fillMaxWidth_ = fill;
        return *this;
    }
    Modifier& fillMaxHeight(bool fill = true) {
        fillMaxHeight_ = fill;
        return *this;
    }
    Modifier& fillMaxSize(bool fill = true) {
        fillMaxWidth_ = fill;
        fillMaxHeight_ = fill;
        return *this;
    }

    // —— 显式尺寸 (优先级高于 fillMax) ——
    Modifier& size(int w, int h) {
        width_ = w;
        height_ = h;
        return *this;
    }
    Modifier& width(int w) {
        width_ = w;
        return *this;
    }
    Modifier& height(int h) {
        height_ = h;
        return *this;
    }

    // —— 内边距 ——
    Modifier& padding(int all) {
        paddingLeft_ = paddingTop_ = paddingRight_ = paddingBottom_ = all;
        return *this;
    }
    
    Modifier& padding(int h, int v) {
        paddingLeft_ = paddingRight_ = h;
        paddingTop_ = paddingBottom_ = v;
        return *this;
    }

    // —— 偏移 ——
    Modifier& offset(int x, int y) {
        offsetX_ = x;
        offsetY_ = y;
        return *this;
    }

    // —— 背景 ——
    Modifier& background(::Color color) {
        backgroundColor_ = color;
        return *this;
    }

    // —— 边框 ——
    Modifier& border(int width, ::Color color) {
        borderWidth_ = width;
        borderColor_ = color;
        return *this;
    }

    // —— 圆角 ——
    // —— 可点击 (类似 Modifier.clickable) ——
    Modifier& clickable(std::function<void()> cb) {
        onClick_ = std::move(cb);
        return *this;
    }

    // —— 圆角 ——
    Modifier& roundedCorner(float radius) {
        roundedCorner_ = radius;
        return *this;
    }

    // —— 透明度 ——
    Modifier& alpha(float a) {
        alpha_ = a;
        return *this;
    }

    // —— 裁剪子组件 ——
    Modifier& clipToBounds(bool clip = true) {
        clip_ = clip;
        return *this;
    }

    // ===== 数据成员 =====
    bool fillMaxWidth_ = false;
    bool fillMaxHeight_ = false;
    int width_ = 0;  // 0 = 不强制
    int height_ = 0; // 0 = 不强制
    int paddingLeft_ = 0;
    int paddingTop_ = 0;
    int paddingRight_ = 0;
    int paddingBottom_ = 0;
    int offsetX_ = 0;
    int offsetY_ = 0;
    ::Color backgroundColor_ = BLANK; // alpha=0 不绘制
    int borderWidth_ = 0;
    ::Color borderColor_ = BLANK;
    float alpha_ = 1.0f;
    float roundedCorner_ = 0.0f; // 0 = 直角
    bool clip_ = false;
    std::function<void()> onClick_; // clickable 回调
};

} // namespace sgui
