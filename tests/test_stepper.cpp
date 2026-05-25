#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& quantity = ctx.rememberState<int>([]() { return 1; });
    auto& percent = ctx.rememberState<int>([]() { return 50; });
    auto& fontSize = ctx.rememberState<int>([]() { return 14; });

    return column({
        text("Stepper Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Quantity (1-99, step 1):", Modifier().padding(8)),
        row({
            stepper(quantity, 1, 99, 1, Modifier().padding(8)),
            text("Value: " + std::to_string(quantity.get()), Modifier().padding(8)),
        }, 10, Arrangement::Start, Alignment::Center),

        text("Percentage (0-100, step 5):", Modifier().padding(8)),
        row({
            stepper(percent, 0, 100, 5, Modifier().padding(8)),
            text("Value: " + std::to_string(percent.get()) + "%", Modifier().padding(8)),
        }, 10, Arrangement::Start, Alignment::Center),

        text("Font Size (8-72, step 2):", Modifier().padding(8)),
        row({
            stepper(fontSize, 8, 72, 2, Modifier().padding(8)),
            text("Value: " + std::to_string(fontSize.get()) + "px", Modifier()
                .padding(8)
                .size(-1, (int)(fontSize.get() * 1.2f))),
        }, 10, Arrangement::Start, Alignment::Center),

        text("", Modifier().padding(4)),
        text("Click +/- to increment/decrement.", Modifier().padding(8)),
        text("Value is clamped to [min, max].", Modifier().padding(4).offset(10, 0)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(500, 400, "Test: Stepper");
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
