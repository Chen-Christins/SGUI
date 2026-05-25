#include "tabs.h"
#include <raylib.h>

namespace sgui {

// ---- TabBar ----

TabBar::TabBar(MutableState<int>& selectedTab, std::vector<std::string> labels)
    : selectedTab_(selectedTab), labels_(std::move(labels)) {}

Size TabBar::measure() const {
    int totalW = 0;
    int maxH = 0;
    for (const auto& label : labels_) {
        int tw = MeasureText(label.c_str(), 20) + kTabPaddingX * 2;
        totalW += tw;
        int th = 30 + kTabPaddingY * 2 + kIndicatorHeight;
        if (th > maxH) maxH = th;
    }
    return {totalW, maxH};
}

bool TabBar::wantsMaxWidth() const { return true; }

void TabBar::render(RenderContext& ctx) {
    int startX = ctx.x;
    int startY = ctx.y;
    int availableW = ctx.maxWidth > 0 ? ctx.maxWidth : measure().width;
    int sel = selectedTab_.get();
    Vector2 mp = GetMousePosition();

    // Compute tab widths (equal distribution)
    int n = (int)labels_.size();
    int tabW = n > 0 ? availableW / n : 0;

    int cx = ctx.x;
    for (int i = 0; i < n; i++) {
        Rectangle tabRect = {(float)cx, (float)ctx.y, (float)tabW, 30.0f + kTabPaddingY * 2};
        bool hovered = CheckCollisionPointRec(mp, tabRect);
        bool isSel = (i == sel);

        // Tab background
        Color bg = isSel ? Color{240, 240, 240, 255} : (hovered ? LIGHTGRAY : RAYWHITE);
        DrawRectangleRec(tabRect, bg);

        // Bottom separator line
        DrawLine(cx, ctx.y + tabRect.height, cx + tabW, ctx.y + tabRect.height, LIGHTGRAY);

        // Selection indicator
        if (isSel) {
            DrawRectangle(cx, ctx.y + tabRect.height - kIndicatorHeight, tabW, kIndicatorHeight, BLUE);
        }

        // Label
        int textW = MeasureText(labels_[i].c_str(), 20);
        int tx = cx + (tabW - textW) / 2;
        int ty = ctx.y + kTabPaddingY + 5;
        DrawText(labels_[i].c_str(), tx, ty, 20, isSel ? DARKBLUE : DARKGRAY);

        // Click
        if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedTab_.set(i);
        }

        cx += tabW;
    }

    ctx.x = startX + availableW;
    ctx.y = startY + 30 + kTabPaddingY * 2 + kIndicatorHeight;
}

// ---- TabPanel ----

TabPanel::TabPanel(int index, MutableState<int>& selectedTab, std::shared_ptr<Widget> content)
    : index_(index), selectedTab_(selectedTab), content_(std::move(content)) {}

Size TabPanel::measure() const {
    if (selectedTab_.get() == index_) {
        return content_->measure();
    }
    return {0, 0};
}

void TabPanel::render(RenderContext& ctx) {
    if (selectedTab_.get() == index_) {
        content_->render(ctx);
    }
}

} // namespace sgui
