#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& dialogOpen = ctx.rememberState<bool>([]() { return false; });
    auto& btnClicked = ctx.rememberState<int>([]() { return -1; });
    auto& lastResult = ctx.rememberState<std::string>([]() { return std::string("(none)"); });

    auto& confirmOpen = ctx.rememberState<bool>([]() { return false; });
    auto& confirmBtn = ctx.rememberState<int>([]() { return -1; });
    auto& confirmResult = ctx.rememberState<std::string>([]() { return std::string("(none)"); });

    return column({
        text("Dialog Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        button("Open Simple Dialog", [&]() { dialogOpen.set(true); },
               Modifier().padding(8)),

        text("Last result: " + lastResult.get(), Modifier()
            .padding(8)),

        separator(false, Modifier().fillMaxWidth().padding(8)),

        button("Open Confirm Dialog", [&]() { confirmOpen.set(true); },
               Modifier().padding(8)),

        text("Confirm result: " + confirmResult.get(), Modifier()
            .padding(8)),

        // Simple dialog with Save/Cancel
        dialog(dialogOpen, "Edit Item",
               column({
                   text("Item Name: example.txt", Modifier().padding(4)),
                   text("Size: 1.2 KB", Modifier().padding(4)),
                   text("Type: Text File", Modifier().padding(4)),
               }),
               {"Save", "Cancel"},
               btnClicked, Modifier()),  // no modifier needed for overlay

        // Result handling
        text(btnClicked.get() == 0 ? "Saved!" : (btnClicked.get() == 1 ? "Cancelled." : ""),
             Modifier().padding(4).offset(10, 0)),

        // Confirm dialog
        dialog(confirmOpen, "Confirm Delete",
               text("Are you sure you want to delete this item?\nThis action cannot be undone.",
                    Modifier().padding(8)),
               {"Delete", "Cancel"},
               confirmBtn, Modifier()),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(600, 500, "Test: Dialog");
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
