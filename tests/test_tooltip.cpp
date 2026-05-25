#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& hoverBtn1 = ctx.remember<bool>([]() { return false; });
    auto& hoverBtn2 = ctx.remember<bool>([]() { return false; });
    auto& hoverText = ctx.remember<bool>([]() { return false; });

    return column({
        text("Tooltip Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Hover over items to see tooltips:", Modifier().padding(8)),

        row({
            text("Button A", Modifier()
                .padding(8).background(LIGHTGRAY, 4).border(1, GRAY)
                .hoverable(hoverBtn1)),
            text("Button B", Modifier()
                .padding(8).background(LIGHTGRAY, 4).border(1, GRAY)
                .hoverable(hoverBtn2)),
            text("Info Icon", Modifier()
                .padding(8).background(LIGHTGRAY, 4).border(1, GRAY)
                .hoverable(hoverText)),
        }, 10, Arrangement::Start, Alignment::Start, Modifier().padding(8)),

        // Tooltips (always at end of tree so they render on top)
        tooltip(hoverBtn1, text("Save your changes", Modifier())),
        tooltip(hoverBtn2, text("Load from disk", Modifier())),
        tooltip(hoverText, text("Click for more information\nabout this setting", Modifier())),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(500, 250, "Test: Tooltip");
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
