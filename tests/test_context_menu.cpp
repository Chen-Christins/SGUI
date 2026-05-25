#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& menuOpen = ctx.rememberState<bool>([]() { return false; });
    auto& menuX = ctx.remember<int>([]() { return 0; });
    auto& menuY = ctx.remember<int>([]() { return 0; });
    auto& selected = ctx.rememberState<int>([]() { return -1; });
    auto& result = ctx.rememberState<std::string>([]() { return std::string("Right-click anywhere"); });

    // Right-click detection
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mp = GetMousePosition();
        menuX = (int)mp.x;
        menuY = (int)mp.y;
        menuOpen.set(true);
    }

    // Update result when item selected
    std::string labels[] = {"New File", "Open...", "Save", "Save As...", "Exit"};
    if (selected.get() >= 0 && selected.get() < 5) {
        result.set("Selected: " + labels[selected.get()]);
    }

    return column({
        text("ContextMenu Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text(result.get(), Modifier().padding(8)),

        separator(false, Modifier().fillMaxWidth().padding(8)),

        text("Right-click anywhere to open menu.", Modifier().padding(8)),
        text("Left-click a menu item to select it.", Modifier().padding(4).offset(10, 0)),
        text("Left-click outside menu to close it.", Modifier().padding(4).offset(10, 0)),

        // Must be last in tree to render on top
        contextMenu(menuOpen, menuX, menuY,
                    {"New File", "Open...", "Save", "Save As...", "-----", "Exit"},
                    selected),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(550, 350, "Test: ContextMenu");
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
