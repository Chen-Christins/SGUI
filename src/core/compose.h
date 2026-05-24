#pragma once

#include <memory>
#include <vector>
#include <functional>

#include "widget.h"
#include "modifier.h"
#include "layout.h"
#include "text_field.h"
#include "checkbox.h"
#include "remember.h"

namespace sgui {

// Helper functions to create widgets in a declarative way
inline std::shared_ptr<Widget> text(const std::string& str, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Text>(str));
}

inline std::shared_ptr<Widget> button(const std::string& str, std::function<void()> onClick, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Button>(str, std::move(onClick)));
}

inline std::shared_ptr<Widget> column(std::vector<std::shared_ptr<Widget>> children, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Column>(std::move(children)));
}

inline std::shared_ptr<Widget> row(std::vector<std::shared_ptr<Widget>> children, int spacing = 10, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Row>(std::move(children), spacing));
}

inline std::shared_ptr<Widget> padding(int pad, std::shared_ptr<Widget> child, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Padding>(pad, std::move(child)));
}

inline std::shared_ptr<Widget> box(std::vector<std::shared_ptr<Widget>> children, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Box>(std::move(children)));
}

inline std::shared_ptr<Widget> spacer() {
    return std::make_shared<Spacer>();
}

inline std::shared_ptr<Widget> textField(
    MutableState<int>& focusedId, int id,
    MutableState<std::string>& textState,
    CompositionContext& ctx,
    std::string placeholder = "",
    Modifier modifier = Modifier())
{
    auto& cursorPos = ctx.remember<int>([]() { return 0; });
    return modifier.then(std::make_shared<TextField>(
        focusedId, id, textState, cursorPos, std::move(placeholder)));
}

inline std::shared_ptr<Widget> checkbox(
    MutableState<bool>& checkedState,
    std::string label,
    Modifier modifier = Modifier())
{
    return modifier.then(std::make_shared<Checkbox>(checkedState, std::move(label)));
}

} // namespace sgui
