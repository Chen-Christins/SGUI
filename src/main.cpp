#include <string>
#include <memory>

#include <raylib.h>

#include <core/state.h>
#include <core/widget.h>
#include <core/compose.h>
#include <core/remember.h>
#include <core/composition_local.h>

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
    static auto label = derivedStateOf([&]() {
        return "Counter value is: " + std::to_string(counter.get());
    });
    label.dependsOn(counter);

    return column({
        // Rounded background title bar
        text("SGUI Reactive Framework", Modifier()
            .padding(16)
            .background(BLUE, 8)
            .fillMaxWidth()),

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
        }, 10, Modifier().padding(8)),

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
        }, 10, Modifier().padding(8)),

        // Checkbox demo
        checkbox(darkMode, "Dark Mode", Modifier()
            .padding(8)),

        text(darkMode.get() ? "Dark mode is ON" : "Dark mode is OFF", Modifier()
            .padding(4)
            .offset(28, 0)),

        // CompositionLocal demo: provides RED color for the subtree
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
    });
}

int main() {
    InitWindow(800, 800, "SGUI - Reactive C++ UI");
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
        BeginDrawing();
        ClearBackground(RAYWHITE);

        RenderContext renderCtx = { 50, 50, 700, 550 };
        auto uiTree = currentUiTree;
        if (uiTree) {
            uiTree->render(renderCtx);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
