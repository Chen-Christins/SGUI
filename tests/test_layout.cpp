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
        text("Layout Arrangement & Alignment Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        // Start (default)
        text("Arrangement.Start", Modifier().padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        row({
            text("A", Modifier().padding(4).background(GRAY, 4)),
            text("B", Modifier().padding(4).background(GRAY, 4)),
            text("C", Modifier().padding(4).background(GRAY, 4)),
        }, 10, Arrangement::Start, Alignment::Start, Modifier().padding(8).fillMaxWidth()),

        // Center
        text("Arrangement.Center", Modifier().padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        row({
            text("A", Modifier().padding(4).background(GRAY, 4)),
            text("B", Modifier().padding(4).background(GRAY, 4)),
            text("C", Modifier().padding(4).background(GRAY, 4)),
        }, 10, Arrangement::Center, Alignment::Start, Modifier().padding(8).fillMaxWidth()),

        // End
        text("Arrangement.End", Modifier().padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        row({
            text("A", Modifier().padding(4).background(GRAY, 4)),
            text("B", Modifier().padding(4).background(GRAY, 4)),
            text("C", Modifier().padding(4).background(GRAY, 4)),
        }, 10, Arrangement::End, Alignment::Start, Modifier().padding(8).fillMaxWidth()),

        // SpaceBetween
        text("Arrangement.SpaceBetween", Modifier().padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        row({
            text("Left", Modifier().padding(4).background(GRAY, 4)),
            text("Center", Modifier().padding(4).background(GRAY, 4)),
            text("Right", Modifier().padding(4).background(GRAY, 4)),
        }, 0, Arrangement::SpaceBetween, Alignment::Start, Modifier().padding(8).fillMaxWidth()),

        // SpaceAround
        text("Arrangement.SpaceAround", Modifier().padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        row({
            text("A", Modifier().padding(4).background(GRAY, 4)),
            text("B", Modifier().padding(4).background(GRAY, 4)),
            text("C", Modifier().padding(4).background(GRAY, 4)),
        }, 0, Arrangement::SpaceAround, Alignment::Start, Modifier().padding(8).fillMaxWidth()),

        // SpaceEvenly
        text("Arrangement.SpaceEvenly", Modifier().padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        row({
            text("A", Modifier().padding(4).background(GRAY, 4)),
            text("B", Modifier().padding(4).background(GRAY, 4)),
            text("C", Modifier().padding(4).background(GRAY, 4)),
        }, 0, Arrangement::SpaceEvenly, Alignment::Start, Modifier().padding(8).fillMaxWidth()),

        // Cross-axis alignment: Center
        text("Alignment.Center (varied heights)", Modifier().padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        row({
            text("Short", Modifier().padding(4).background(GRAY, 4)),
            text("Taller", Modifier().padding(8).size(100, 50).background(GRAY, 4)),
            text("Mid", Modifier().padding(6).background(GRAY, 4)),
        }, 10, Arrangement::Start, Alignment::Center, Modifier().padding(8)),

        // Cross-axis alignment: End
        text("Alignment.End (varied heights)", Modifier().padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        row({
            text("Short", Modifier().padding(4).background(GRAY, 4)),
            text("Taller", Modifier().padding(8).size(100, 50).background(GRAY, 4)),
            text("Mid", Modifier().padding(6).background(GRAY, 4)),
        }, 10, Arrangement::Start, Alignment::End, Modifier().padding(8)),

        // Per-child align override
        text("Per-child AlignModifier", Modifier().padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        row({
            text("Top", Modifier().padding(4).background(GRAY, 4)),
            text("Center", Modifier().padding(4).background(GRAY, 4).align(Alignment::Center)),
            text("Bottom", Modifier().padding(4).background(GRAY, 4).align(Alignment::End)),
            text("Tall-Box", Modifier().size(80, 60).background(GRAY, 4)),
        }, 10, Arrangement::Start, Alignment::Start, Modifier().padding(8)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 700, "Test: Layout");
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
