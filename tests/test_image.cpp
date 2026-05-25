#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

// Generate a procedural checkerboard texture for testing
static Texture2D MakeCheckerboard(int size, int tiles) {
    Image img = GenImageColor(size, size, BLANK);
    int tileSize = size / tiles;
    for (int y = 0; y < tiles; y++) {
        for (int x = 0; x < tiles; x++) {
            Color c = ((x + y) % 2 == 0) ? RED : BLUE;
            ImageDrawRectangle(&img, x * tileSize, y * tileSize, tileSize, tileSize, c);
        }
    }
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

std::shared_ptr<Widget> App(Texture2D& checker, CompositionContext& ctx) {
    return column({
        text("Image Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        // Natural size
        text("Natural size (64x64):", Modifier().padding(8)),
        imageView(checker, Modifier().padding(8)),

        // Explicit size
        text("Explicit size (128x64):", Modifier().padding(8)),
        imageView(checker, 128, 64, Modifier().padding(8)),

        // Fill max width
        text("fillMaxWidth:", Modifier()
            .padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        imageView(checker, Modifier()
            .padding(8).fillMaxWidth()),

        // Fill max size (constrained height)
        text("fillMaxWidth + size(height=80):", Modifier()
            .padding(8).fillMaxWidth().background(LIGHTGRAY, 4)),
        imageView(checker, Modifier()
            .padding(8).fillMaxWidth().size(-1, 80)),

        // Border around image
        text("With border:", Modifier().padding(8)),
        imageView(checker, 100, 100, Modifier()
            .padding(8).border(3, DARKGRAY)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(600, 650, "Test: Image");
    SetTargetFPS(60);

    Texture2D checker = MakeCheckerboard(64, 4);

    CompositionContext compCtx;
    std::shared_ptr<Widget> uiTree;

    auto rebuild = [&]() {
        compCtx.reset();
        uiTree = App(checker, compCtx);
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

    UnloadTexture(checker);
    CloseWindow();
    return 0;
}
