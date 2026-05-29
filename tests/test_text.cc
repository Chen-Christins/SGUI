/**
 * @file test_text.cc
 * @brief 测试文本组件
 * @author Christins (chen.christins@qq.com)
 * @date 2026-05-29
 * @copyright Apache 2.0
 */
#include <core/window.hpp>
#include <core/text.hpp>

int main() {
    sgui::Window app(800, 600, "Text Test");
    app.setTargetFPS(60);

    // 渲染相关的对象创建和资源加载，必须放在主循环（app.run）之外！
    // 否则每秒执行 60 次字体加载、且在 BeginDrawing 期间加载 GPU 纹理会导致黑块和内存泄漏
    sgui::Text text("你好，SGUI！组件内部自动加载字体测试。");
    text.setFontSize(30)
        .setColor(RED)
        .setSpacing(2.0f)
        .loadFont("./assets/fonts/NotoSansCJKsc-Regular.otf"); 

    app.run([&]() {
        sgui::RenderContext ctx{100, 100, 800, 600};
        text.render(ctx);
    });

    return 0;
}