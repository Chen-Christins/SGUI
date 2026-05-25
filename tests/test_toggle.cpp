#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& wifi = ctx.rememberState<bool>([]() { return true; });
    auto& bluetooth = ctx.rememberState<bool>([]() { return false; });
    auto& darkMode = ctx.rememberState<bool>([]() { return false; });
    auto& notifications = ctx.rememberState<bool>([]() { return true; });

    return column({
        text("Toggle Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        // Toggle with label
        row({
            text("WiFi:", Modifier().padding(8)),
            toggle(wifi, Modifier().padding(8)),
            text(wifi.get() ? "ON" : "OFF", Modifier().padding(8)),
        }, 10, Arrangement::Start, Alignment::Center),

        row({
            text("Bluetooth:", Modifier().padding(8)),
            toggle(bluetooth, Modifier().padding(8)),
            text(bluetooth.get() ? "ON" : "OFF", Modifier().padding(8)),
        }, 10, Arrangement::Start, Alignment::Center),

        row({
            text("Dark Mode:", Modifier().padding(8)),
            toggle(darkMode, Modifier().padding(8)),
            text(darkMode.get() ? "ON" : "OFF", Modifier().padding(8)),
        }, 10, Arrangement::Start, Alignment::Center),

        row({
            text("Notifications:", Modifier().padding(8)),
            toggle(notifications, Modifier().padding(8)),
            text(notifications.get() ? "ON" : "OFF", Modifier().padding(8)),
        }, 10, Arrangement::Start, Alignment::Center),

        // Styled toggle inside a padded container
        text("", Modifier().padding(4)),
        row({
            text("Styled:", Modifier().padding(8)),
            toggle(darkMode, Modifier()
                .padding(4)
                .background(LIGHTGRAY, 4)
                .border(2, GRAY)),
            text("(with background + border)", Modifier().padding(8)),
        }, 10, Arrangement::Start, Alignment::Center),

        text("", Modifier().padding(8)),

        // Toggle status summary
        text("--- Status ---", Modifier()
            .padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        text("WiFi: " + std::string(wifi.get() ? "ON" : "off"), Modifier()
            .padding(4).offset(10, 0)),
        text("Bluetooth: " + std::string(bluetooth.get() ? "ON" : "off"), Modifier()
            .padding(4).offset(10, 0)),
        text("Dark Mode: " + std::string(darkMode.get() ? "ON" : "off"), Modifier()
            .padding(4).offset(10, 0)),
        text("Notifications: " + std::string(notifications.get() ? "ON" : "off"), Modifier()
            .padding(4).offset(10, 0)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(500, 450, "Test: Toggle");
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
