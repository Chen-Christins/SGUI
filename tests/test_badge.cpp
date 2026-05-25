#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& inboxCount = ctx.rememberState<int>([]() { return 5; });
    auto& alertsCount = ctx.rememberState<int>([]() { return 42; });
    auto& errorCount = ctx.rememberState<int>([]() { return 120; });

    return column({
        text("Badge Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        row({
            badge(inboxCount.get(),
                  text("Inbox", Modifier()
                      .padding(8).size(80, 28).background(LIGHTGRAY, 4)),
                  Modifier().padding(8)),

            badge(alertsCount.get(),
                  text("Alerts", Modifier()
                      .padding(8).size(80, 28).background(LIGHTGRAY, 4)),
                  Modifier().padding(8)),

            badge(errorCount.get(),
                  text("Errors", Modifier()
                      .padding(8).size(80, 28).background(LIGHTGRAY, 4)),
                  Modifier().padding(8)),
        }, 10, Arrangement::Start, Alignment::Start, Modifier().padding(8)),

        text("", Modifier().padding(4)),
        text("Badges show notification count on widgets.", Modifier().padding(8)),
        text("Count > 99 displays as '99+'.", Modifier().padding(4).offset(10, 0)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(500, 250, "Test: Badge");
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
