/**
 * @file widget.h
 * @brief 基础组件接口定义
 * @author Christins (chen.christins@qq.com)
 * @date 2026-05-29
 * @copyright Apache 2.0
 */
#pragma once

#include <raylib.h>
#include "modifier.hpp"

namespace sgui {

struct RenderContext {
    int x;
    int y;
    int maxWidth;
    int maxHeight;
};

struct Size {
    int width;
    int height;
};

class Widget {
public:
    virtual ~Widget() = default;
    virtual void render(RenderContext& ctx) = 0;
    virtual Size measure() const { return {}; }
    
    // 直接代理读取 Modifier 里的属性
    bool isFillMaxWidth() const { return modifier.fillMaxWidth_; }
    bool isFillMaxHeight() const { return modifier.fillMaxHeight_; }
    
    Modifier modifier; // 提取出来的修饰器
};

} // namespace sgui
