#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(State<int>& counter, CompositionContext& ctx) {
    auto& pressCount = ctx.rememberState<int>([]() { return 0; });

    return column({
        text("Button Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Counter: " + std::to_string(counter.get()), Modifier().padding(8)),

        // Default button
        button("Default", [&]() {
            pressCount.set(pressCount.get() + 1);
        }, Modifier().padding(8)),

        // Styled button with background + border
        button("Styled (Rounded BG + Border)", [&]() {
            counter.set(counter.get() + 1);
        }, Modifier()
            .padding(4)
            .size(220, 40)
            .background(GRAY, 6)
            .border(2, DARKGRAY)
            .padding(8)),

        // Small button
        button("Small", [&]() {
            pressCount.set(pressCount.get() + 1);
        }, Modifier()
            .padding(2)
            .size(80, 30)
            .padding(8)),

        // Full-width button
        button("Full Width", [&]() {
            counter.set(counter.get() + 1);
        }, Modifier()
            .padding(4)
            .fillMaxWidth()
            .background(LIGHTGRAY, 4)
            .padding(8)),

        // Clickable text
        text("Clicks: " + std::to_string(pressCount.get()), Modifier()
            .padding(8)
            .clickable([&]() { pressCount.set(pressCount.get() + 1); })
            .background(LIGHTGRAY, 4)),

        // Row of buttons with SpaceBetween
        row({
            button("Left", [&]() {}, Modifier().padding(4).size(100, 40).background(GRAY, 4)),
            button("Center", [&]() {}, Modifier().padding(4).size(100, 40).background(GRAY, 4)),
            button("Right", [&]() {}, Modifier().padding(4).size(100, 40).background(GRAY, 4)),
        }, 10, Arrangement::SpaceBetween, Alignment::Start,
           Modifier().padding(8).fillMaxWidth()),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 500, "Test: Button");
    SetTargetFPS(60);

    State<int> counter(0);
    CompositionContext compCtx;
    std::shared_ptr<Widget> uiTree;

    auto rebuild = [&]() {
        compCtx.reset();
        uiTree = App(counter, compCtx);
    };
    compCtx.setInvalidate(rebuild);
    counter.observe([&](const int&) { rebuild(); });
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
