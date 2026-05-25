#include "dialog.h"
#include <raylib.h>

namespace sgui {

Dialog::Dialog(MutableState<bool>& isOpen, std::string title,
               std::shared_ptr<Widget> content,
               std::vector<std::string> buttonLabels,
               MutableState<int>& buttonClicked)
    : isOpen_(isOpen), title_(std::move(title)), content_(std::move(content)),
      buttonLabels_(std::move(buttonLabels)), buttonClicked_(buttonClicked) {}

Size Dialog::measure() const { return {0, 0}; }

void Dialog::render(RenderContext& ctx) {
    if (!isOpen_.get()) return;

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // Backdrop
    DrawRectangle(0, 0, sw, sh, Fade(BLACK, 0.5f));

    // Panel sizing
    Size contentSize = content_->measure();
    int panelW = kPanelWidth;
    int btnRowH = buttonLabels_.empty() ? 0 : kButtonHeight + kPadding * 2;
    int contentH = contentSize.height + kTitleHeight + btnRowH + kPadding * 2;
    int panelH = contentH > 150 ? contentH : 200;

    int panelX = (sw - panelW) / 2;
    int panelY = (sh - panelH) / 2;

    // Panel background — two-layer
    Rectangle panel = {(float)panelX, (float)panelY, (float)panelW, (float)panelH};
    DrawRectangleRounded(panel, (float)kCornerRadius, 16, GRAY);
    Rectangle inner = {panel.x + 1.5f, panel.y + 1.5f, panel.width - 3.0f, panel.height - 3.0f};
    DrawRectangleRounded(inner, (float)(kCornerRadius - 1), 16, RAYWHITE);

    // Title bar
    Rectangle titleBar = {(float)panelX, (float)panelY, (float)panelW, (float)kTitleHeight};
    DrawRectangleRounded({titleBar.x, titleBar.y, titleBar.width, titleBar.height},
                         (float)kCornerRadius, 16, DARKBLUE);
    DrawRectangle(titleBar.x, titleBar.y + kTitleHeight / 2.0f,
                  titleBar.width, titleBar.height / 2, DARKBLUE);
    DrawText(title_.c_str(), panelX + kPadding, panelY + 12, 20, WHITE);

    Vector2 mp = GetMousePosition();

    // Content
    int contentY = panelY + kTitleHeight + kPadding;
    RenderContext contentCtx = {panelX + kPadding, contentY, panelW - kPadding * 2, -1};
    content_->render(contentCtx);

    // Buttons
    int btnY = contentCtx.y + kPadding;
    if (!buttonLabels_.empty()) {
        int n = (int)buttonLabels_.size();
        int btnW = (panelW - kPadding * (n + 1)) / n;
        for (int i = 0; i < n; i++) {
            int bx = panelX + kPadding + i * (btnW + kPadding);
            Rectangle btnRect = {(float)bx, (float)btnY, (float)btnW, (float)kButtonHeight};
            bool hoverBtn = CheckCollisionPointRec(mp, btnRect);

            Color btnBg = hoverBtn ? BLUE : Color{220, 220, 220, 255};
            DrawRectangleRounded(btnRect, 4.0f, 16, GRAY);
            Rectangle btnInner = {btnRect.x + 1.0f, btnRect.y + 1.0f,
                                  btnRect.width - 2.0f, btnRect.height - 2.0f};
            DrawRectangleRounded(btnInner, 3.0f, 16, btnBg);

            int tw = MeasureText(buttonLabels_[i].c_str(), 20);
            DrawText(buttonLabels_[i].c_str(), bx + (btnW - tw) / 2, btnY + 8, 20,
                     hoverBtn ? WHITE : DARKGRAY);

            if (hoverBtn && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                buttonClicked_.set(i);
                isOpen_.set(false);
            }
        }
    }

    // ctx unchanged (overlay)
}

} // namespace sgui
