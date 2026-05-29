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

/// 圆角矩形点击区域检测 (9-区域法)
inline bool hitTestRoundedRect(int px, int py, ::Rectangle bounds, float radius) {
    if (!CheckCollisionPointRec({(float)px, (float)py}, bounds)) return false;
    if (radius <= 0.0f) return true;

    float l = bounds.x, t = bounds.y;
    float w = bounds.width, h = bounds.height;
    float r = radius;

    // 核心矩形区 (内缩 r)：一定命中
    if (px >= l + r && px <= l + w - r && py >= t + r && py <= t + h - r) return true;

    // 四个角弧形区：距离检测
    auto inCorner = [](int px, int py, float cx, float cy, float r) {
        float dx = px - cx, dy = py - cy;
        return (dx * dx + dy * dy) <= (r * r);
    };
    if (px < l + r && py < t + r)       return inCorner(px, py, l + r, t + r, r);
    if (px > l + w - r && py < t + r)   return inCorner(px, py, l + w - r, t + r, r);
    if (px < l + r && py > t + h - r)   return inCorner(px, py, l + r, t + h - r, r);
    if (px > l + w - r && py > t + h - r) return inCorner(px, py, l + w - r, t + h - r, r);

    // 边缘平坦区：一定命中
    return true;
}

struct Size {
    int width;
    int height;
};

class Widget {
public:
    virtual ~Widget() = default;
    virtual void render(RenderContext& ctx) = 0;
    virtual Size measure() const { return {}; }

    /// 鼠标按下事件，返回 true 表示事件已消费
    virtual bool onMouseDown(int x, int y) { return false; }
    
    // 直接代理读取 Modifier 里的属性
    bool isFillMaxWidth() const { return modifier.fillMaxWidth_; }
    bool isFillMaxHeight() const { return modifier.fillMaxHeight_; }
    
    Modifier modifier; // 提取出来的修饰器
};

} // namespace sgui
