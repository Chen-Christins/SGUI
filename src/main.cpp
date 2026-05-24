#include <string>
#include <memory>
#include "raylib.h"
#include "SGUI/State.h"
#include "SGUI/Widget.h"
#include "SGUI/Compose.h"

using namespace sgui;

// A composable function that builds UI based on state
std::shared_ptr<Widget> App(State<int>& counterState) {
    return column({
        text("Hello, SGUI Reactive Framework!"),
        text("Counter value is: " + std::to_string(counterState.get())),
        button("Increment", [&]() { 
            counterState.set(counterState.get() + 1); 
        }),
        button("Decrement", [&]() { 
            counterState.set(counterState.get() - 1); 
        })
    });
}

int main() {
    // Initialize the Raylib window
    InitWindow(800, 600, "SGUI - Reactive C++ UI");
    SetTargetFPS(60);

    // Application State
    State<int> counter(0);
    std::shared_ptr<Widget> currentUiTree;

    // When state changes, re-evaluate the UI component tree
    counter.observe([&](const int& val){
        currentUiTree = App(counter);
    });

    // Main game/UI loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Create a render context with initial padding
        RenderContext ctx = { 50, 50 };
        
        // Keep the current tree alive during the entire render,
        // even if an observer replaces currentUiTree mid-render (e.g. on button click).
        auto uiTree = currentUiTree;
        if (uiTree) {
            uiTree->render(ctx);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
