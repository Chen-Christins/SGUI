#include "text_field.h"
#include <cmath>
#include <raylib.h>

namespace sgui {

TextField::TextField(MutableState<int>& focusedId, int id, MutableState<std::string>& textState, int& cursorPos,
                     std::string placeholder)
    : focusedId_(focusedId), id_(id), textState_(textState), cursorPos_(cursorPos), placeholder_(std::move(placeholder)) {}

Size TextField::measure() const { return {200, 40}; }

void TextField::render(RenderContext& ctx) {
    bool isFocused = (focusedId_.get() == id_);
    const std::string& text = textState_.get();

    Rectangle bounds = {(float)ctx.x, (float)ctx.y, 200.0f, 40.0f};

    // Background
    DrawRectangleRec(bounds, isFocused ? WHITE : LIGHTGRAY);

    // Border
    if (isFocused) {
        DrawRectangleLinesEx(bounds, 2.0f, BLUE);
    } else {
        DrawRectangleLinesEx(bounds, 1.0f, GRAY);
    }

    // Click-to-focus and cursor placement
    Vector2 mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        focusedId_.set(id_);

        // Approximate cursor position from click x
        int relX = (int)mousePoint.x - ctx.x - 5;
        int bestPos = 0;
        int bestDist = 100000;
        int len = (int)text.length();
        for (int i = 0; i <= len; i++) {
            int cx = MeasureText(text.substr(0, i).c_str(), 20);
            int dist = std::abs(cx - relX);
            if (dist < bestDist) {
                bestDist = dist;
                bestPos = i;
            }
        }
        cursorPos_ = bestPos;
    }

    // Handle keyboard input when focused
    if (isFocused) {
        handleInput();
    }

    // Clamp cursor
    int len = (int)text.length();
    if (cursorPos_ < 0) {
        cursorPos_ = 0;
    }
    if (cursorPos_ > len) {
        cursorPos_ = len;
    }

    // Draw text or placeholder
    int textX = ctx.x + 5;
    int textY = ctx.y + 10;

    if (text.empty() && !isFocused) {
        DrawText(placeholder_.c_str(), textX, textY, 20, GRAY);
    } else {
        DrawText(text.c_str(), textX, textY, 20, BLACK);
    }

    // Draw blinking cursor
    if (isFocused) {
        double t = GetTime();
        bool visible = (std::fmod(t, 1.0) < 0.5);
        if (visible) {
            int cursorX = textX + MeasureText(text.substr(0, cursorPos_).c_str(), 20);
            DrawLine(cursorX, ctx.y + 8, cursorX, ctx.y + 32, BLACK);
        }
    }

    ctx.x += 200;
    ctx.y += 40;
}

void TextField::handleInput() {
    // Character input (handles keyboard layout, shift, etc.)
    int ch = GetCharPressed();
    while (ch > 0) {
        if (ch >= 32 && ch <= 126) {
            insertChar(ch);
        }
        ch = GetCharPressed();
    }

    // Key input — GetKeyPressed is reliable for initial press detection.
    // Also check IsKeyPressedRepeat for OS-level key repeat on held keys.
    int key = GetKeyPressed();
    while (key > 0) {
        switch (key) {
        case KEY_BACKSPACE:
            deleteBeforeCursor();
            break;
        case KEY_DELETE:
            deleteAfterCursor();
            break;
        case KEY_LEFT:
            moveCursorLeft();
            break;
        case KEY_RIGHT:
            moveCursorRight();
            break;
        case KEY_HOME:
            cursorPos_ = 0;
            break;
        case KEY_END:
            cursorPos_ = (int)textState_.get().length();
            break;
        default:
            break;
        }
        key = GetKeyPressed();
    }

    // Handle repeats for held-down nav/edit keys
    if (IsKeyPressedRepeat(KEY_BACKSPACE)) {
        deleteBeforeCursor();
    }
    if (IsKeyPressedRepeat(KEY_DELETE)) {
        deleteAfterCursor();
    }
    if (IsKeyPressedRepeat(KEY_LEFT)) {
        moveCursorLeft();
    }
    if (IsKeyPressedRepeat(KEY_RIGHT)) {
        moveCursorRight();
    }
}

void TextField::insertChar(int ch) {
    std::string text = textState_.get();
    text.insert(cursorPos_, 1, (char)ch);
    cursorPos_++;
    textState_.set(std::move(text));
}

void TextField::deleteBeforeCursor() {
    if (cursorPos_ > 0) {
        std::string text = textState_.get();
        text.erase(cursorPos_ - 1, 1);
        cursorPos_--;
        textState_.set(std::move(text));
    }
}

void TextField::deleteAfterCursor() {
    std::string text = textState_.get();
    int len = (int)text.length();
    if (cursorPos_ < len) {
        text.erase(cursorPos_, 1);
        textState_.set(std::move(text));
    }
}

void TextField::moveCursorLeft() {
    if (cursorPos_ > 0) {
        cursorPos_--;
    }
}

void TextField::moveCursorRight() {
    int len = (int)textState_.get().length();
    if (cursorPos_ < len) {
        cursorPos_++;
    }
}

} // namespace sgui
