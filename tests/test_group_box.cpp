#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& name = ctx.rememberState<std::string>([]() { return std::string("John"); });
    auto& email = ctx.rememberState<std::string>([]() { return std::string("john@example.com"); });
    auto& focusedId = ctx.rememberState<int>([]() { return -1; });

    return column({
        text("GroupBox Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        groupBox("Personal Info",
            column({
                row({
                    text("Name:", Modifier().padding(8).size(60, 20)),
                    textField(focusedId, 1, name, ctx,
                              "", Modifier().fillMaxWidth()),
                }, 4, Arrangement::Start, Alignment::Center, Modifier()
                    .fillMaxWidth().padding(4)),
                row({
                    text("Email:", Modifier().padding(8).size(60, 20)),
                    textField(focusedId, 2, email, ctx,
                              "", Modifier().fillMaxWidth()),
                }, 4, Arrangement::Start, Alignment::Center, Modifier()
                    .fillMaxWidth().padding(4)),
            }),
            Modifier().padding(8).fillMaxWidth()
        ),

        groupBox("Preferences",
            column({
                text("Theme: Light", Modifier().padding(4)),
                text("Language: English", Modifier().padding(4)),
                text("Notifications: On", Modifier().padding(4)),
            }),
            Modifier().padding(8).fillMaxWidth()
        ),

        groupBox("Advanced",
            text("Advanced settings content here.\nMore options available.",
                 Modifier().padding(8)),
            Modifier().padding(8).fillMaxWidth()
        ),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(550, 500, "Test: GroupBox");
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
