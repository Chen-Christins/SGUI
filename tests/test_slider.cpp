#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& volume = ctx.rememberState<float>([]() { return 0.5f; });
    auto& brightness = ctx.rememberState<float>([]() { return 0.8f; });
    auto& stepValue = ctx.rememberState<float>([]() { return 0.0f; });
    auto& intValue = ctx.rememberState<float>([]() { return 5.0f; });

    return column({
        text("Slider Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        // Continuous slider (no step)
        text("Volume (continuous 0.0 - 1.0):", Modifier().padding(8)),
        slider(volume, 0.0f, 1.0f, 0.0f, Modifier()
            .padding(8).fillMaxWidth()),
        text("Value: " + std::to_string(volume.get()), Modifier()
            .padding(4).offset(10, 0)),

        // Continuous slider
        text("Brightness (continuous 0.0 - 1.0):", Modifier().padding(8)),
        slider(brightness, 0.0f, 1.0f, 0.0f, Modifier()
            .padding(8).fillMaxWidth()),
        text("Value: " + std::to_string(brightness.get()), Modifier()
            .padding(4).offset(10, 0)),

        // Stepped slider
        text("Stepped (0.0 - 1.0, step 0.1):", Modifier().padding(8)),
        slider(stepValue, 0.0f, 1.0f, 0.1f, Modifier()
            .padding(8).fillMaxWidth()),
        text("Value: " + std::to_string(stepValue.get()), Modifier()
            .padding(4).offset(10, 0)),

        // Integer-style stepped slider
        text("Integer-like (0-10, step 1):", Modifier().padding(8)),
        slider(intValue, 0.0f, 10.0f, 1.0f, Modifier()
            .padding(8).fillMaxWidth()),
        text("Value: " + std::to_string((int)intValue.get()), Modifier()
            .padding(4).offset(10, 0)),

        // Slider in a fixed-size container (not full width)
        text("Fixed-width slider (300px):", Modifier().padding(8)),
        slider(volume, 0.0f, 1.0f, 0.0f, Modifier()
            .padding(8).size(300, 30)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(600, 500, "Test: Slider");
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
