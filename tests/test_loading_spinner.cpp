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
        text("LoadingSpinner Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Small (16px):", Modifier().padding(8)),
        loadingSpinner(16, Modifier().padding(8)),

        text("Default (24px):", Modifier().padding(8)),
        loadingSpinner(24, Modifier().padding(8)),

        text("Medium (32px):", Modifier().padding(8)),
        loadingSpinner(32, Modifier().padding(8)),

        text("Large (48px):", Modifier().padding(8)),
        loadingSpinner(48, Modifier().padding(8)),

        text("", Modifier().padding(4)),
        text("All spinners animate simultaneously.", Modifier().padding(8)),
        text("Green spinner = loading; turns red when done.", Modifier().padding(4).offset(10, 0)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(400, 450, "Test: LoadingSpinner");
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
