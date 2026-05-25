#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& p1 = ctx.rememberState<float>([]() { return 0.0f; });
    auto& p2 = ctx.rememberState<float>([]() { return 0.25f; });
    auto& p3 = ctx.rememberState<float>([]() { return 0.5f; });
    auto& p4 = ctx.rememberState<float>([]() { return 0.75f; });
    auto& p5 = ctx.rememberState<float>([]() { return 1.0f; });
    auto& sliderVal = ctx.rememberState<float>([]() { return 0.6f; });

    return column({
        text("ProgressBar Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Static progress values:", Modifier().padding(8)),

        row({
            text("0%", Modifier().padding(4)),
            progressBar(p1, Modifier().padding(8).fillMaxWidth()),
        }, 8, Arrangement::Start, Alignment::Center),

        row({
            text("25%", Modifier().padding(4)),
            progressBar(p2, Modifier().padding(8).fillMaxWidth()),
        }, 8, Arrangement::Start, Alignment::Center),

        row({
            text("50%", Modifier().padding(4)),
            progressBar(p3, Modifier().padding(8).fillMaxWidth()),
        }, 8, Arrangement::Start, Alignment::Center),

        row({
            text("75%", Modifier().padding(4)),
            progressBar(p4, Modifier().padding(8).fillMaxWidth()),
        }, 8, Arrangement::Start, Alignment::Center),

        row({
            text("100%", Modifier().padding(4)),
            progressBar(p5, Modifier().padding(8).fillMaxWidth()),
        }, 8, Arrangement::Start, Alignment::Center),

        // Interactive progress controlled by slider
        text("", Modifier().padding(4)),
        text("Interactive (drag slider to change):", Modifier().padding(8)),

        row({
            text(std::to_string((int)(sliderVal.get() * 100)) + "%", Modifier()
                .padding(4).size(50, 20)),
            slider(sliderVal, 0.0f, 1.0f, 0.01f, Modifier()
                .fillMaxWidth().padding(8)),
        }, 8, Arrangement::Start, Alignment::Center, Modifier()
            .padding(8).fillMaxWidth()),

        progressBar(sliderVal, Modifier()
            .padding(8).fillMaxWidth()),

        // Fixed-width progress bar
        text("", Modifier().padding(4)),
        text("Fixed-width (300px):", Modifier().padding(8)),
        progressBar(sliderVal, Modifier()
            .padding(8).size(300, 20)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(600, 550, "Test: ProgressBar");
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
