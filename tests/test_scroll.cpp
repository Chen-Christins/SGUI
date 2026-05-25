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
        text("Scroll Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Vertical Scroll (mouse wheel inside area)", Modifier()
            .padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),

        // Vertical scroll with many items
        verticalScroll(
            column({
                text("Item 01", Modifier().padding(4)),
                text("Item 02", Modifier().padding(4)),
                text("Item 03", Modifier().padding(4)),
                text("Item 04", Modifier().padding(4)),
                text("Item 05", Modifier().padding(4)),
                text("Item 06", Modifier().padding(4)),
                text("Item 07", Modifier().padding(4)),
                text("Item 08", Modifier().padding(4)),
                text("Item 09", Modifier().padding(4)),
                text("Item 10", Modifier().padding(4)),
                text("Item 11", Modifier().padding(4)),
                text("Item 12", Modifier().padding(4)),
                text("Item 13", Modifier().padding(4)),
                text("Item 14", Modifier().padding(4)),
                text("Item 15 (last)", Modifier().padding(4)),
            }),
            ctx, true,
            Modifier().fillMaxWidth().size(-1, 200)
        ),

        text("Horizontal Scroll (shift+wheel or trackpad swipe)", Modifier()
            .padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),

        // Horizontal scroll with wide row
        horizontalScroll(
            row({
                text("Column-01", Modifier().padding(8).size(140, 40).background(GRAY, 4)),
                text("Column-02", Modifier().padding(8).size(140, 40).background(GRAY, 4)),
                text("Column-03", Modifier().padding(8).size(140, 40).background(GRAY, 4)),
                text("Column-04", Modifier().padding(8).size(140, 40).background(GRAY, 4)),
                text("Column-05", Modifier().padding(8).size(140, 40).background(GRAY, 4)),
                text("Column-06", Modifier().padding(8).size(140, 40).background(GRAY, 4)),
                text("Column-07", Modifier().padding(8).size(140, 40).background(GRAY, 4)),
                text("Column-08", Modifier().padding(8).size(140, 40).background(GRAY, 4)),
            }, 4),
            ctx, true,
            Modifier().fillMaxWidth()
        ),

        text("Vertical Scroll (hidden scrollbar)", Modifier()
            .padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),

        verticalScroll(
            column({
                text("A scroll without scrollbar", Modifier().padding(4)),
                text("Item 1", Modifier().padding(4)),
                text("Item 2", Modifier().padding(4)),
                text("Item 3", Modifier().padding(4)),
                text("Item 4", Modifier().padding(4)),
                text("Item 5", Modifier().padding(4)),
                text("Item 6", Modifier().padding(4)),
                text("Item 7", Modifier().padding(4)),
            }),
            ctx, false,  // showScrollbar = false
            Modifier().fillMaxSize()
        ),
    }, 5, Arrangement::Start, Alignment::Start, Modifier().fillMaxSize());
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(700, 650, "Test: Scroll");
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
