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

    // 2. 构建 Row，演示主轴等距排列 (SpaceEvenly) 和交叉轴居中对齐 (Center)
    auto row = std::make_shared<sgui::Row>();
    row->fillMaxWidth(true)
        .setHorizontalArrangement(sgui::Arrangement::SpaceBetween)
        .setVerticalAlignment(sgui::Alignment::Center)
        .addChild(text1)
        .addChild(text2)
        .addChild(text3);

    // 3. 构建 Column，作为根，自动撑满并全局居中
    auto column = std::make_shared<sgui::Column>();
    column->fillMaxWidth(true)
          .fillMaxHeight(true)
          .setVerticalArrangement(sgui::Arrangement::End)
          .setHorizontalAlignment(sgui::Alignment::Start) // 内部组件都在主轴上居中
          .addChild(title)
        //   .addChild(std::make_shared<sgui::Spacer>(0, 60)) // Spacer 产生 60px 的垂直间距
          .addChild(row);

    app.run([&]() {
        // 直接传递根组件的 RenderContext（0, 0 坐标，窗口宽高）
        sgui::RenderContext ctx{0, 0, 800, 600};
        column->render(ctx);
    });

    return 0;
}