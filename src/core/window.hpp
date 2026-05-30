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
    Window(int width, int height, int minWidth, int minHeight, int maxWidth, int maxHeight, const std::string& title);
    ~Window();

    void setTargetFPS(int fps);
    void setBackgroundColor(Color color);
    
    void setMinSize(int width, int height);
    void setMaxSize(int width, int height);

    void run(const std::function<void()>& renderFunc);
    void run(std::shared_ptr<Widget> rootWidget);

private:
    int width_;
    int height_;
    std::string title_;
    Color bgColor_ = RAYWHITE; 
};

} // namespace sgui