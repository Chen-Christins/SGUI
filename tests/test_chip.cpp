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
        text("Chip Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Default style:", Modifier().padding(8)),

        row({
            chip("Design", Color{200, 220, 255, 255}, Modifier().padding(4)),
            chip("Code", Color{200, 255, 200, 255}, Modifier().padding(4)),
            chip("Test", Color{255, 220, 200, 255}, Modifier().padding(4)),
            chip("Deploy", Color{220, 200, 255, 255}, Modifier().padding(4)),
        }, 8, Arrangement::Start, Alignment::Start, Modifier().padding(8)),

        text("Dark chips:", Modifier().padding(8)),

        row({
            chip("React", BLUE, Modifier().padding(4)),
            chip("Vue", DARKGREEN, Modifier().padding(4)),
            chip("Angular", RED, Modifier().padding(4)),
        }, 8, Arrangement::Start, Alignment::Start, Modifier().padding(8)),

        text("", Modifier().padding(4)),
        text("Chips are compact label/tag elements.", Modifier().padding(8)),
        text("Text color auto-adapts to background brightness.", Modifier().padding(4).offset(10, 0)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(500, 300, "Test: Chip");
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
