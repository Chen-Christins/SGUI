#pragma once

#include "remember.h"
#include "widget.h"
#include <string>

namespace sgui {

// Editable text input widget.
// State (text + cursor) is stored in MutableState so it survives recomposition.
class TextField : public Widget {
public:
    // focusedId: shared MutableState tracking which widget has focus.
    // id: unique id for this TextField instance.
    // textState: the editable text value for this field.
    // cursorPos: persisted cursor position (from CompositionContext::remember).
    // placeholder: shown when text is empty and field is not focused.
    TextField(MutableState<int>& focusedId, int id, MutableState<std::string>& textState, int& cursorPos,
              std::string placeholder = "");

    void render(RenderContext& ctx) override;
    Size measure() const override;

private:
    void handleInput();
    void insertChar(int ch);
    void deleteBeforeCursor();
    void deleteAfterCursor();
    void moveCursorLeft();
    void moveCursorRight();

    MutableState<int>& focusedId_;
    int id_;
    MutableState<std::string>& textState_;
    int& cursorPos_;
    std::string placeholder_;
};

} // namespace sgui
