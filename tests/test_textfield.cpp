#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& focusedId = ctx.rememberState<int>([]() { return -1; });
    auto& nameState = ctx.rememberState<std::string>([]() { return std::string(""); });
    auto& emailState = ctx.rememberState<std::string>([]() { return std::string(""); });

    return column({
        text("TextField Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Click a field to focus, type to edit, arrows/backspace to navigate.",
             Modifier().padding(8)),

        // Name field
        text("Name:", Modifier().padding(8)),
        textField(focusedId, 1, nameState, ctx, "Enter your name...",
                  Modifier().padding(8)),

        // Email field
        text("Email:", Modifier().padding(8)),
        textField(focusedId, 2, emailState, ctx, "Enter your email...",
                  Modifier().padding(8)),

        // Show field values
        text("Name value: \"" + nameState.get() + "\"", Modifier().padding(8)),
        text("Email value: \"" + emailState.get() + "\"", Modifier().padding(8)),

        // Hint
        text("(tab between fields not yet supported)", Modifier()
            .padding(4).offset(10, 0)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 450, "Test: TextField");
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
