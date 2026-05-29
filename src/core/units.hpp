/**
 * @file units.hpp
 * @brief 密度无关单位系统 (dp/px 转换 + 字面量)
 * @author Christins (chen.christins@qq.com)
 * @copyright Apache 2.0
 */
#pragma once

namespace sgui {

// 全局密度缩放: 1dp = gDensity px (默认 1.0, Retina 设为 2.0 等)
inline float gDensity = 1.0f;

inline void setDensity(float d) { gDensity = d; }

inline int dp2px(int dp) { return static_cast<int>(dp * gDensity); }
inline int px2dp(int px) { return static_cast<int>(px / gDensity); }

/// 函数式 dp 值构造: dp(16) 等价于 16dp 转 px
inline int dp(int v) { return dp2px(v); }
inline int px(int v) { return v; }  // 显式 px，恒等，语义占位

// —— 字面量 (可选, 需 using sgui::unit::operator""_dp) ——
namespace unit {
inline int operator""_dp(unsigned long long v) { return dp2px(static_cast<int>(v)); }
} // namespace unit

} // namespace sgui
