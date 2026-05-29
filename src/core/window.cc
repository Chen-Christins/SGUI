#include "window.hpp"

namespace sgui {

Window::Window(int width, int height, const std::string& title)
    : width_(width)
    , height_(height)
    , title_(title) {
    SetTraceLogLevel(LOG_WARNING);
    // 同时开启 MSAA 和 Mac 视网膜高清屏(HighDPI)支持，这是解决 Mac 模糊的核心
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(width_, height_, title_.c_str());
}

Window::~Window() {
    CloseWindow();
}

void Window::setTargetFPS(int fps) {
    SetTargetFPS(fps);
}

void Window::setBackgroundColor(Color color) {
    bgColor_ = color;
}

void Window::run(const std::function<void()>& renderFunc) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(bgColor_);
        
        renderFunc();
        
        EndDrawing();
    }
}

void Window::run(std::shared_ptr<Widget> rootWidget) {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(bgColor_);
        
        if (rootWidget) {
            RenderContext ctx{0, 0, width_, height_};
            rootWidget->render(ctx);
        }
        
        EndDrawing();
    }
}

} // namespace sgui