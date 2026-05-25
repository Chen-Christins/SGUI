#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& colorGroup = ctx.rememberState<int>([]() { return 0; });
    auto& sizeGroup = ctx.rememberState<int>([]() { return 1; });

    return column({
        text("RadioButton Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Color Selection:", Modifier().padding(8)),
        radioButton(colorGroup, 0, "Red", Modifier().padding(4).offset(8, 0)),
        radioButton(colorGroup, 1, "Green", Modifier().padding(4).offset(8, 0)),
        radioButton(colorGroup, 2, "Blue", Modifier().padding(4).offset(8, 0)),

        text("Selected: " + std::string(
            colorGroup.get() == 0 ? "Red" : colorGroup.get() == 1 ? "Green" : "Blue"),
            Modifier().padding(8).offset(28, 0)),

        separator(false, Modifier().fillMaxWidth().padding(8)),

        text("Size Selection:", Modifier().padding(8)),
        radioButton(sizeGroup, 0, "Small", Modifier().padding(4).offset(8, 0)),
        radioButton(sizeGroup, 1, "Medium", Modifier().padding(4).offset(8, 0)),
        radioButton(sizeGroup, 2, "Large", Modifier().padding(4).offset(8, 0)),
        radioButton(sizeGroup, 3, "Extra Large", Modifier().padding(4).offset(8, 0)),

        text("Selected: " + std::string(
            sizeGroup.get() == 0 ? "Small" : sizeGroup.get() == 1 ? "Medium" :
            sizeGroup.get() == 2 ? "Large" : "Extra Large"),
            Modifier().padding(8).offset(28, 0)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(500, 400, "Test: RadioButton");
    SetTargetFPS(60);

    CompositionContext compCtx;
    std::shared_ptr<Widget> uiTree;

    auto rebuild = [&]() {
        compCtx.reset();
        uiTree = App(compCtx);
    };
    compCtx.setInvalidate(rebuild);
    rebuild();

    while (!WindowShouldClose()) {
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        RenderContext rctx = { 0, 0, sw, sh };
        auto tree = uiTree;
        if (tree) { tree->render(rctx); }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
