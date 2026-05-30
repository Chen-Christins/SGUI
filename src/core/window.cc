#include "window.hpp"

namespace sgui {

Window::Window(int width, int height, const std::string& title)
    : width_(width)
    , height_(height)
    , title_(title) {
    SetTraceLogLevel(LOG_WARNING);
    // 同时开启 MSAA 和 Mac 视网膜高清屏(HighDPI)支持，以及允许缩放
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(width_, height_, title_.c_str());
}

Window::Window(int width, int height, int minWidth, int minHeight, int maxWidth, int maxHeight, const std::string& title)
    : width_(width)
    , height_(height)
    , title_(title) {
    SetTraceLogLevel(LOG_WARNING);
    // 同时开启 MSAA 和 Mac 视网膜高清屏(HighDPI)支持，以及允许缩放
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(width_, height_, title_.c_str());
    SetWindowMinSize(minWidth, minHeight);
    SetWindowMaxSize(maxWidth, maxHeight);
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

void Window::setMinSize(int width, int height) {
    SetWindowMinSize(width, height);
}

void Window::setMaxSize(int width, int height) {
    SetWindowMaxSize(width, height);
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
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            rootWidget->onMouseDown((int)mouse.x, (int)mouse.y);
        }

        BeginDrawing();
        ClearBackground(bgColor_);

        if (rootWidget) {
            RenderContext ctx{0, 0, GetScreenWidth(), GetScreenHeight()};
            rootWidget->render(ctx);
        }

        EndDrawing();
    }
}

} // namespace sgui