/**
 * @file modifier.hpp
 * @brief 组件修饰器
 * @author Christins (chen.christins@qq.com)
 * @copyright Apache 2.0
 */
#pragma once

#include <raylib.h>

namespace sgui {

class Modifier {
public:
    // 链式调用设定 Modifier
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

    Modifier& background(::Color color) {
        backgroundColor_ = color;
        return *this;
    }

    bool fillMaxWidth_ = false;
    bool fillMaxHeight_ = false;
    ::Color backgroundColor_ = BLANK; // alpha=0 表示不绘制背景
};

} // namespace sgui