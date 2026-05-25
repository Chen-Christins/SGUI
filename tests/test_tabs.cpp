#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& activeTab = ctx.rememberState<int>([]() { return 0; });

    return column({
        text("Tab Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        tabBar(activeTab, {"Home", "Profile", "Settings"}, Modifier()
            .fillMaxWidth()),

        tabPanel(0, activeTab,
            column({
                text("Welcome to Home!", Modifier().padding(16)),
                text("This is the home tab content.", Modifier().padding(4).offset(10, 0)),
                text("Tab content switches instantly on click.", Modifier().padding(4).offset(10, 0)),
            })
        ),

        tabPanel(1, activeTab,
            column({
                text("User Profile", Modifier().padding(16)),
                text("Name: John Doe", Modifier().padding(4).offset(10, 0)),
                text("Email: john@example.com", Modifier().padding(4).offset(10, 0)),
                text("Role: Developer", Modifier().padding(4).offset(10, 0)),
            })
        ),

        tabPanel(2, activeTab,
            column({
                text("Settings", Modifier().padding(16)),
                text("Theme: Light", Modifier().padding(4).offset(10, 0)),
                text("Notifications: Enabled", Modifier().padding(4).offset(10, 0)),
                text("Language: English", Modifier().padding(4).offset(10, 0)),
                text("Version: 1.0.0", Modifier().padding(4).offset(10, 0)),
            })
        ),

        separator(false, Modifier().fillMaxWidth().padding(8)),

        // Show active tab info
        text("Active Tab Index: " + std::to_string(activeTab.get()), Modifier()
            .padding(8)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(550, 400, "Test: Tabs");
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
