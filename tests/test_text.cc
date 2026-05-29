/**
 * @file test_text.cc
 * @brief 测试文本组件
 * @author Christins (chen.christins@qq.com)
 * @date 2026-05-29
 * @copyright Apache 2.0
 */
#include <core/window.hpp>

int main() {
    sgui::Window app(800, 600, "Text Test");
    app.setTargetFPS(60);

    app.run([&]() {
        
    });

    return 0;
}