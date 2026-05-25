#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& optA = ctx.rememberState<bool>([]() { return false; });
    auto& optB = ctx.rememberState<bool>([]() { return true; });
    auto& optC = ctx.rememberState<bool>([]() { return false; });
    auto& all = ctx.rememberState<bool>([]() { return false; });

    return column({
        text("Checkbox Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        checkbox(optA, "Option A (default off)", Modifier().padding(8)),
        checkbox(optB, "Option B (default on)", Modifier().padding(8)),
        checkbox(optC, "Option C (default off)", Modifier().padding(8)),

        text("---", Modifier().padding(8)),

        text("Option A: " + std::string(optA.get() ? "ON" : "off"), Modifier().padding(4).offset(28, 0)),
        text("Option B: " + std::string(optB.get() ? "ON" : "off"), Modifier().padding(4).offset(28, 0)),
        text("Option C: " + std::string(optC.get() ? "ON" : "off"), Modifier().padding(4).offset(28, 0)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(500, 350, "Test: Checkbox");
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
