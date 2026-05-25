#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/composition_local.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

// A custom theme color
CompositionLocal<Color> LocalAccentColor{BLUE};

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    return column({
        text("CompositionLocal Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        // Default text color (DARKGRAY)
        text("Default text color (from LocalTextColor default)", Modifier().padding(8)),

        // Override with RED
        LocalTextColor.provides(RED,
            text("This text is RED (LocalTextColor override)", Modifier().padding(8))
        ),

        // Override with GREEN
        LocalTextColor.provides(GREEN,
            column({
                text("This text is GREEN", Modifier().padding(4)),
                text("So is this one", Modifier().padding(4)),
            })
        ),

        // Nested override: outer RED, inner BLUE
        LocalTextColor.provides(RED,
            column({
                text("This is RED (outer)", Modifier().padding(4)),
                LocalTextColor.provides(BLUE,
                    text("This is BLUE (inner override)", Modifier().padding(4))
                ),
                text("Back to RED (outer)", Modifier().padding(4)),
            })
        ),

        // Back to default
        text("Back to default color", Modifier().padding(8)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(600, 450, "Test: CompositionLocal");
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
