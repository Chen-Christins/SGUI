#include "context_menu.h"
#include <raylib.h>

namespace sgui {

ContextMenu::ContextMenu(MutableState<bool>& isOpen, int& screenX, int& screenY,
                         std::vector<std::string> items,
                         MutableState<int>& selectedItem)
    : isOpen_(isOpen), screenX_(screenX), screenY_(screenY),
      items_(std::move(items)), selectedItem_(selectedItem) {}

Size ContextMenu::measure() const { return {0, 0}; }

void ContextMenu::render(RenderContext& ctx) {
    if (!isOpen_.get()) return;

    // Compute menu width
    int menuW = kMinWidth;
    for (const auto& item : items_) {
        int tw = MeasureText(item.c_str(), 20) + kItemPaddingX * 2;
        if (tw > menuW) menuW = tw;
    }

    int menuH = (int)items_.size() * kItemHeight + 4;
    int x = screenX_;
    int y = screenY_;

    // Keep on screen
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    if (x + menuW > sw) x = sw - menuW - 4;
    if (y + menuH > sh) y = sh - menuH - 4;

    // Menu background — two-layer
    Rectangle box = {(float)x, (float)y, (float)menuW, (float)menuH};
    DrawRectangleRounded(box, 4.0f, 16, DARKGRAY);
    Rectangle inner = {box.x + 1.0f, box.y + 1.0f, box.width - 2.0f, box.height - 2.0f};
    DrawRectangleRounded(inner, 3.0f, 16, RAYWHITE);

    Vector2 mp = GetMousePosition();

    // Items
    for (int i = 0; i < (int)items_.size(); i++) {
        Rectangle itemRect = {(float)x + 2, (float)y + 2 + (float)i * kItemHeight,
                              (float)menuW - 4, (float)kItemHeight};
        bool hoverItem = CheckCollisionPointRec(mp, itemRect);

        if (hoverItem) {
            DrawRectangleRec(itemRect, Color{230, 230, 235, 255});
        }
        DrawText(items_[i].c_str(), x + kItemPaddingX, y + 2 + i * kItemHeight + 4, 20,
                 DARKGRAY);

        if (hoverItem && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedItem_.set(i);
            isOpen_.set(false);
        }
    }

    // Click outside closes
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(mp, box)) {
        isOpen_.set(false);
    }

    // ctx unchanged (overlay)
}

} // namespace sgui
