# SGUI - Reactive C++ UI Framework

A lightweight, reactive UI framework for C++17, built on [raylib](https://www.raylib.com/). SGUI brings a declarative, state-driven component model to native GUI development.

## Features

- **Reactive State** — `State<T>` container automatically rebuilds the UI when data changes
- **Declarative Composition** — Compose widget trees with `text()`, `button()`, `column()` helpers
- **Immediate-mode Rendering** — Leverages raylib for cross-platform, GPU-accelerated drawing
- **Header-only Core** — Widgets, state management, and composition helpers are all in headers

## Quick Start

### Prerequisites

- CMake 3.14+
- C++17 compiler (Clang, GCC, or MSVC)
- System libraries for raylib (see [raylib wiki](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux))

**macOS:**
```bash
# No extra dependencies — raylib uses system frameworks
```

**Linux (Debian/Ubuntu):**
```bash
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
```

### Build

```bash
cmake -B build
cmake --build build
./build/SGUI
```

## Example

```cpp
#include "SGUI/State.h"
#include "SGUI/Widget.h"
#include "SGUI/Compose.h"

using namespace sgui;

std::shared_ptr<Widget> App(State<int>& counter) {
    return column({
        text("Counter: " + std::to_string(counter.get())),
        button("+1", [&] { counter.set(counter.get() + 1); }),
        button("-1", [&] { counter.set(counter.get() - 1); })
    });
}

int main() {
    InitWindow(800, 600, "SGUI Demo");
    SetTargetFPS(60);

    State<int> counter(0);
    std::shared_ptr<Widget> uiTree;

    counter.observe([&](const int&) {
        uiTree = App(counter);
    });

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        RenderContext ctx = { 50, 50 };
        auto tree = uiTree;
        if (tree) tree->render(ctx);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

## Widgets

| Widget   | Description                        |
|----------|------------------------------------|
| `Text`   | Renders a text label               |
| `Button` | Clickable button with hover effect |
| `Column` | Vertical layout container          |

### Custom Widgets

Subclass `Widget` and implement `render(RenderContext& ctx)`:

```cpp
class MyWidget : public Widget {
public:
    void render(RenderContext& ctx) override {
        // Draw using raylib, advance ctx.y for layout
    }
};
```

## License

MIT
