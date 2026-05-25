#include "dropdown.h"
#include <raylib.h>

namespace sgui {

Dropdown::Dropdown(MutableState<int>& selectedIndex, std::vector<std::string> options, bool& isOpen)
    : selectedIndex_(selectedIndex), options_(std::move(options)), isOpen_(isOpen) {}

Size Dropdown::measure() const {
    int h = kButtonHeight;
    if (isOpen_) {
        h += kItemHeight * (int)options_.size();
    }
    return {kButtonWidth, h};
}

void Dropdown::render(RenderContext& ctx) {
    int startX = ctx.x;
    int startY = ctx.y;
    int w = ctx.maxWidth > 0 ? ctx.maxWidth : kButtonWidth;

    int sel = selectedIndex_.get();
    const char* displayText = (sel >= 0 && sel < (int)options_.size()) ? options_[sel].c_str() : "Select...";

    // Button — two-layer filled
    Rectangle btn = {(float)ctx.x, (float)ctx.y, (float)w, (float)kButtonHeight};
    Vector2 mp = GetMousePosition();
    bool hoverBtn = CheckCollisionPointRec(mp, btn);

    Color btnBg = hoverBtn ? LIGHTGRAY : Color{220, 220, 220, 255};
    DrawRectangleRounded(btn, 4.0f, 16, GRAY);
    Rectangle btnInner = {btn.x + 1.0f, btn.y + 1.0f, btn.width - 2.0f, btn.height - 2.0f};
    DrawRectangleRounded(btnInner, 3.0f, 16, btnBg);

    // Click button to toggle
    if (hoverBtn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isOpen_ = !isOpen_;
    }

    // Button text
    DrawText(displayText, ctx.x + 8, ctx.y + 10, 20, DARKGRAY);

    // Dropdown arrow indicator
    int arrowX = ctx.x + w - 24;
    int arrowY = ctx.y + kButtonHeight / 2;
    DrawTriangle({(float)arrowX, (float)arrowY - 3},
                 {(float)arrowX + 8, (float)arrowY - 3},
                 {(float)arrowX + 4, (float)arrowY + 4}, DARKGRAY);

    ctx.y += kButtonHeight;

    // Open: render list items
    if (isOpen_) {
        for (int i = 0; i < (int)options_.size(); i++) {
            Rectangle itemRect = {(float)ctx.x, (float)ctx.y, (float)w, (float)kItemHeight};
            bool hoverItem = CheckCollisionPointRec(mp, itemRect);

            Color itemBg = (i == sel) ? BLUE : (hoverItem ? LIGHTGRAY : WHITE);
            Color itemTextColor = (i == sel) ? WHITE : DARKGRAY;

            DrawRectangleRec(itemRect, itemBg);
            if (i < (int)options_.size() - 1) {
                DrawLine(ctx.x + 4, ctx.y + kItemHeight - 1, ctx.x + w - 4, ctx.y + kItemHeight - 1,
                         Fade(GRAY, 0.3f));
            }

            DrawText(options_[i].c_str(), ctx.x + 8, ctx.y + 6, 20, itemTextColor);

            if (hoverItem && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedIndex_.set(i);
                isOpen_ = false;
            }

            ctx.y += kItemHeight;
        }
    }

    ctx.x = startX + w;
}

} // namespace sgui
