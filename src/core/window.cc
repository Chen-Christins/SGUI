#include "window.hpp"

namespace sgui {

Window::Window(int width, int height, const std::string& title)
    : width_(width)
    , height_(height)
    , title_(title) {
    SetTraceLogLevel(LOG_WARNING);
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