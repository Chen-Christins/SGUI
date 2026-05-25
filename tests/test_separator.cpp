#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    return column({
        text("Separator Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Horizontal separators:", Modifier().padding(8)),

        text("Above separator", Modifier().padding(8)),
        separator(false, Modifier().fillMaxWidth().padding(8)),
        text("Below separator", Modifier().padding(8)),

        separator(false, Modifier().fillMaxWidth().padding(8)),

        text("Another section", Modifier().padding(8)),

        separator(false, Modifier().fillMaxWidth().padding(8)),

        text("Vertical separators in a row:", Modifier().padding(8)),

        row({
            text("Left", Modifier().padding(8)),
            separator(true, Modifier().padding(4)),
            text("Center", Modifier().padding(8)),
            separator(true, Modifier().padding(4)),
            text("Right", Modifier().padding(8)),
        }, 10, Arrangement::Start, Alignment::Start, Modifier()
            .padding(8)),

        separator(false, Modifier().fillMaxWidth().padding(8)),

        // Vertical separators with different heights
        row({
            text("Short", Modifier().padding(4)),
            separator(true, Modifier().padding(4).size(1, 30)),
            text("Taller Box", Modifier().padding(8).size(100, 50).background(LIGHTGRAY, 4)),
            separator(true, Modifier().padding(4).size(1, 50)),
            text("End", Modifier().padding(4)),
        }, 10, Arrangement::Start, Alignment::Center, Modifier()
            .padding(8)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(500, 450, "Test: Separator");
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
