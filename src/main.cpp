#include <memory>
#include <string>

#include <raylib.h>

#include <core/compose.h>
#include <core/composition_local.h>
#include <core/remember.h>
#include <core/state.h>
#include <core/widget.h>

using namespace sgui;

// A composable function that builds the full UI tree
std::shared_ptr<Widget> App(State<int>& counter, CompositionContext& ctx) {
    // Local mutable state — triggers rebuild when set() is called
    auto& localCount = ctx.rememberState<int>([]() { return 0; });

    // Focus coordination for text fields
    auto& focusedId = ctx.rememberState<int>([]() { return -1; });

    // Text field states
    auto& nameState = ctx.rememberState<std::string>([]() { return std::string(""); });

    // Checkbox state
    auto& darkMode = ctx.rememberState<bool>([]() { return false; });

    // Derived label text — recomputed only when counter changes
    static auto label = derivedStateOf([&]() { return "Counter value is: " + std::to_string(counter.get()); });
    label.dependsOn(counter);

    return column({
        // Fixed title bar (stays at top, not scrollable)
        text("SGUI Reactive Framework", Modifier()
            .padding(16)
            .background(BLUE, 8)
            .fillMaxWidth()),

        // Scrollable body — takes remaining window height
        verticalScroll(
            column({
                text(label.get(), Modifier()
                    .padding(8)),

                // Border-style button row
                row({
                    button("Increment", [&]() {
                        counter.set(counter.get() + 1);
                    }, Modifier()
                        .padding(4)
                        .size(120, 40)
                        .background(GRAY, 4)
                        .border(2, DARKGRAY)),

                    spacer(),

                    button("Decrement", [&]() {
                        counter.set(counter.get() - 1);
                    }, Modifier()
                        .padding(4)
                        .size(120, 40)
                        .background(GRAY, 4)
                        .border(2, DARKGRAY)),
                }, 10, Arrangement::Start, Alignment::Start,
                   Modifier().padding(8)),

                // Arrangement demo: SpaceBetween row
                text("Row Arrangement.SpaceBetween:", Modifier()
                    .padding(8).fillMaxWidth()
                    .background(LIGHTGRAY, 4)),

                row({
                    text("Left", Modifier().padding(4).background(GRAY, 4)),
                    text("Center", Modifier().padding(4).background(GRAY, 4)),
                    text("Right", Modifier().padding(4).background(GRAY, 4)),
                }, 0, Arrangement::SpaceBetween, Alignment::Start,
                   Modifier().padding(8).fillMaxWidth()),

                // Alignment demo: row with varied heights, Center alignment
                text("Row Alignment.Center:", Modifier()
                    .padding(8).fillMaxWidth()
                    .background(LIGHTGRAY, 4)),

                row({
                    text("Short", Modifier().padding(4).background(GRAY, 4)),
                    text("Taller Item", Modifier()
                        .padding(8).size(100, 50)
                        .background(GRAY, 4)),
                    text("Mid", Modifier().padding(6).background(GRAY, 4)),
                }, 10, Arrangement::Start, Alignment::Center,
                   Modifier().padding(8)),

                // SpaceEvenly demo
                text("Row Arrangement.SpaceEvenly:", Modifier()
                    .padding(8).fillMaxWidth()
                    .background(LIGHTGRAY, 4)),

                row({
                    text("A", Modifier().padding(4).background(GRAY, 4)),
                    text("B", Modifier().padding(4).background(GRAY, 4)),
                    text("C", Modifier().padding(4).background(GRAY, 4)),
                }, 0, Arrangement::SpaceEvenly, Alignment::Start,
                   Modifier().padding(8).fillMaxWidth()),

                // Offset demo: hint text slightly shifted
                text("(buttons have rounded border)", Modifier()
                    .padding(4)
                    .offset(10, 0)),

                // Box with border
                box({
                    text("Box Background", Modifier()
                        .background(LIGHTGRAY, 4)
                        .border(2, GRAY)
                        .size(260, 60)),

                    text("Box Foreground", Modifier()
                        .padding(4)),
                }, Modifier().padding(8)),

                row({
                    text("Local: " + std::to_string(localCount.get()), Modifier()
                        .padding(4)),

                    button("Local +", [&]() {
                        localCount.set(localCount.get() + 1);
                    }, Modifier()
                        .padding(4)
                        .border(1, DARKGRAY)),
                }, 10, Arrangement::Start, Alignment::Start,
                   Modifier().padding(8)),

                // Checkbox demo
                checkbox(darkMode, "Dark Mode", Modifier()
                    .padding(8)),

                text(darkMode.get() ? "Dark mode is ON" : "Dark mode is OFF", Modifier()
                    .padding(4)
                    .offset(28, 0)),

                // CompositionLocal demo
                LocalTextColor.provides(RED,
                    text("This text is wrapped in CompositionLocal(RED)", Modifier()
                        .padding(8))
                ),

                // TextField demo
                text("Name Input:", Modifier()
                    .padding(8)),

                textField(focusedId, 1, nameState, ctx, "Enter your name...", Modifier()
                    .padding(8)),

                text("Hello, " + nameState.get() + "!", Modifier()
                    .padding(8)),

                // Extra items to force overflow and demonstrate scrolling
                text("--- Scroll Demo: extra items below ---", Modifier()
                    .padding(8).fillMaxWidth()
                    .background(LIGHTGRAY, 4)),

                text("Item 1: this demonstrates scrolling", Modifier().padding(8)),
                text("Item 2: more content below", Modifier().padding(8)),
                text("Item 3: keep scrolling", Modifier().padding(8)),
                text("Item 4: almost there", Modifier().padding(8)),
                text("Item 5: bottom of scrollable content", Modifier().padding(8)),

                // Horizontal scroll demo
                text("--- Horizontal Scroll Demo ---", Modifier()
                    .padding(8).fillMaxWidth()
                    .background(LIGHTGRAY, 4)),

                horizontalScroll(
                    row({
                        text("Col-1", Modifier().padding(8).size(150, 40).background(GRAY, 4)),
                        text("Col-2", Modifier().padding(8).size(150, 40).background(GRAY, 4)),
                        text("Col-3", Modifier().padding(8).size(150, 40).background(GRAY, 4)),
                        text("Col-4", Modifier().padding(8).size(150, 40).background(GRAY, 4)),
                        text("Col-5", Modifier().padding(8).size(150, 40).background(GRAY, 4)),
                        text("Col-6", Modifier().padding(8).size(150, 40).background(GRAY, 4)),
                    }, 8),
                    ctx,
                    true,
                    Modifier().fillMaxWidth()
                ),
            }),
            ctx,
            true,
            Modifier().fillMaxSize()
        ),
    });
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "SGUI - Reactive C++ UI");
    SetTargetFPS(60);

    State<int> counter(0);
    std::shared_ptr<Widget> currentUiTree;
    CompositionContext compCtx;

    auto rebuild = [&]() {
        compCtx.reset();
        currentUiTree = App(counter, compCtx);
    };

    // Wire up the CompositionContext to trigger rebuild on local state changes
    compCtx.setInvalidate(rebuild);
    counter.observe([&](const int&) { rebuild(); });

    while (!WindowShouldClose()) {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        int pad = 0;
        RenderContext renderCtx = {pad, pad, screenW - pad * 2, screenH - pad * 2};
        auto uiTree = currentUiTree;
        if (uiTree) {
            uiTree->render(renderCtx);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
