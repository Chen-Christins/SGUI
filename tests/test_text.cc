/**
 * @file test_text.cc
 * @brief 测试文本组件
 * @author Christins (chen.christins@qq.com)
 * @date 2026-05-29
 * @copyright Apache 2.0
 */
#include <core/window.hpp>
#include <core/text.hpp>
#include <core/row.hpp>
#include <core/column.hpp>
#include <core/box.hpp>
#include <core/spacer.hpp>
#include <core/alignment.hpp>
#include <memory>

int main() {
    sgui::Window app(800, 600, "SGUI Layout Test");
    app.setTargetFPS(60);

    // 1. 创建各个文本组件（此处每个组件内部自动提取自己文本的码位并加载支持高清防锯齿的字体）
    auto title = std::make_shared<sgui::Text>("SGUI 现代布局系统测试 / Layout Test");
    title->setFontSize(36).setColor(DARKBLUE).setSpacing(2.0f).loadFont("./assets/fonts/NotoSansCJKsc-Regular.otf");

    auto text1 = std::make_shared<sgui::Text>("Start");
    text1->setFontSize(24).setColor(RED).loadFont("./assets/fonts/NotoSansCJKsc-Regular.otf");
    
    auto text2 = std::make_shared<sgui::Text>("Center");
    text2->setFontSize(24).setColor(DARKGREEN).loadFont("./assets/fonts/NotoSansCJKsc-Regular.otf");
    
    auto text3 = std::make_shared<sgui::Text>("End");
    text3->setFontSize(24).setColor(ORANGE).loadFont("./assets/fonts/NotoSansCJKsc-Regular.otf");

    // 2. 构建 Row，Modifier 设置 fill 属性，后面跟布局专属参数和子组件
    auto row = sgui::Row::create(
        sgui::Modifier().fillMaxWidth(),
        sgui::Arrangement::SpaceAround,
        sgui::Alignment::Center,
        {
            text1,
            text2,
            text3
        }
    );

    // 3. 构建 Box 包裹标题，演示 2D 内容居中
    auto titleBox = sgui::Box::create(
        sgui::Modifier().fillMaxWidth(),
        sgui::Alignment2D::Center,
        {
            title
        }
    );

    // 4. 构建 Column 根节点，Modifier 设满宽高
    auto column = sgui::Column::create(
        sgui::Modifier().fillMaxSize(),
        sgui::Arrangement::SpaceAround,
        sgui::Alignment::End,
        {
            titleBox,
            row
        }
    );

    app.run([&]() {
        // 直接传递根组件的 RenderContext（0, 0 坐标，窗口宽高）
        sgui::RenderContext ctx{0, 0, 800, 600};
        column->render(ctx);
    });

    return 0;
}