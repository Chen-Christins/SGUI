/**
 * @file alignment.hpp
 * @brief 布局对齐和排列规则 
 * @author Christins (chen.christins@qq.com)
 * @copyright Apache 2.0
 */
#pragma once

namespace sgui {

// 线性布局的主轴排列方式
enum class Arrangement {
    Start,          // 居主轴起点
    Center,         // 居主轴中心
    End,            // 居主轴终点
    SpaceBetween,   // 两端对齐，中间间隙均匀
    SpaceAround,    // 组件四周间隙均匀
    SpaceEvenly     // 完全平均分配间隙
};

// 线性布局的交叉轴对齐
enum class Alignment {
    Start,          // 居交叉轴起点
    Center,         // 居交叉轴中心
    End             // 居交叉轴终点
};

// 2D 堆叠布局的方位对齐
enum class Alignment2D {
    TopStart,
    TopCenter,
    TopEnd,
    CenterStart,
    Center,
    CenterEnd,
    BottomStart,
    BottomCenter,
    BottomEnd
};

} // namespace sgui