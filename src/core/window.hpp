/**
 * @file window.h
 * @brief 窗口组件
 * @author Christins (chen.christins@qq.com)
 * @date 2026-05-30
 * @copyright Apache 2.0
 */
#pragma once

#include "widget.hpp"
#include "utils/nocopyable.hpp"

#include <string>
#include <functional>

namespace sgui {

class Window : public NoCopyable {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    void setTargetFPS(int fps);
    void setBackgroundColor(Color color); // raylib的Color

    // 用法 1：立即调用模式 (传入每次循环需要执行的渲染逻辑)
    void run(const std::function<void()>& renderFunc);

    // 用法 2：保留模式 (挂载一个根组件，Window自动调用其 render 和 measure)
    void run(std::shared_ptr<Widget> rootWidget);

private:
    int width_;
    int height_;
    std::string title_;
    Color bgColor_ = RAYWHITE; 
};

} // namespace sgui