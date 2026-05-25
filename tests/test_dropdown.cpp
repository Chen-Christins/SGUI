#include <memory>
#include <string>
#include <raylib.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

std::shared_ptr<Widget> App(CompositionContext& ctx) {
    auto& fruitIndex = ctx.rememberState<int>([]() { return -1; });
    auto& cityIndex = ctx.rememberState<int>([]() { return 2; });
    auto& fruitOpen = ctx.remember<bool>([]() { return false; });
    auto& cityOpen = ctx.remember<bool>([]() { return false; });

    return column({
        text("Dropdown Test", Modifier()
            .padding(16).background(BLUE, 8).fillMaxWidth()),

        text("Select a fruit (default none):", Modifier().padding(8)),
        dropdown(fruitIndex, {"Apple", "Banana", "Cherry", "Date", "Elderberry"},
                 fruitOpen, Modifier().padding(8)),

        text("Selected: " + std::string(
            fruitIndex.get() < 0 ? "(none)" :
            fruitIndex.get() == 0 ? "Apple" :
            fruitIndex.get() == 1 ? "Banana" :
            fruitIndex.get() == 2 ? "Cherry" :
            fruitIndex.get() == 3 ? "Date" : "Elderberry"),
            Modifier().padding(8).offset(10, 0)),

        separator(false, Modifier().fillMaxWidth().padding(8)),

        text("Select a city (default Tokyo):", Modifier().padding(8)),
        dropdown(cityIndex, {"New York", "London", "Tokyo", "Paris", "Berlin", "Sydney"},
                 cityOpen, Modifier().padding(8)),

        text("Selected: " + std::string(
            cityIndex.get() == 0 ? "New York" :
            cityIndex.get() == 1 ? "London" :
            cityIndex.get() == 2 ? "Tokyo" :
            cityIndex.get() == 3 ? "Paris" :
            cityIndex.get() == 4 ? "Berlin" : "Sydney"),
            Modifier().padding(8).offset(10, 0)),

        separator(false, Modifier().fillMaxWidth().padding(8)),

        text("Click dropdown to expand/collapse.", Modifier().padding(8)),
        text("Select an item to close the dropdown.", Modifier().padding(4).offset(10, 0)),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(550, 500, "Test: Dropdown");
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
