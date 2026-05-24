#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "checkbox.h"
#include "layout.h"
#include "modifier.h"
#include "remember.h"
#include "scroll.h"
#include "text_field.h"
#include "widget.h"

namespace sgui {

// Helper functions to create widgets in a declarative way
inline std::shared_ptr<Widget> text(const std::string& str, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Text>(str));
}

inline std::shared_ptr<Widget> button(const std::string& str, std::function<void()> onClick, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Button>(str, std::move(onClick)));
}

inline std::shared_ptr<Widget> column(std::vector<std::shared_ptr<Widget>> children, int spacing = 5,
                                      Arrangement arrangement = Arrangement::Start, Alignment alignment = Alignment::Start,
                                      Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Column>(std::move(children), spacing, arrangement, alignment));
}

inline std::shared_ptr<Widget> row(std::vector<std::shared_ptr<Widget>> children, int spacing = 10,
                                   Arrangement arrangement = Arrangement::Start, Alignment alignment = Alignment::Start,
                                   Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Row>(std::move(children), spacing, arrangement, alignment));
}

inline std::shared_ptr<Widget> padding(int pad, std::shared_ptr<Widget> child, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Padding>(pad, std::move(child)));
}

inline std::shared_ptr<Widget> box(std::vector<std::shared_ptr<Widget>> children, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Box>(std::move(children)));
}

inline std::shared_ptr<Widget> spacer() { return std::make_shared<Spacer>(); }

inline std::shared_ptr<Widget> textField(MutableState<int>& focusedId, int id, MutableState<std::string>& textState,
                                         CompositionContext& ctx, std::string placeholder = "", Modifier modifier = Modifier()) {
    auto& cursorPos = ctx.remember<int>([]() { return 0; });
    return modifier.then(std::make_shared<TextField>(focusedId, id, textState, cursorPos, std::move(placeholder)));
}

inline std::shared_ptr<Widget> checkbox(MutableState<bool>& checkedState, std::string label, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Checkbox>(checkedState, std::move(label)));
}

inline std::shared_ptr<Widget> verticalScroll(std::shared_ptr<Widget> child, CompositionContext& ctx, bool showScrollbar = true,
                                              Modifier modifier = Modifier()) {
    auto& offset = ctx.remember<float>([]() { return 0.0f; });
    return modifier.then(std::make_shared<VerticalScroll>(offset, std::move(child), showScrollbar));
}

inline std::shared_ptr<Widget> horizontalScroll(std::shared_ptr<Widget> child, CompositionContext& ctx, bool showScrollbar = true,
                                                Modifier modifier = Modifier()) {
    auto& offset = ctx.remember<float>([]() { return 0.0f; });
    return modifier.then(std::make_shared<HorizontalScroll>(offset, std::move(child), showScrollbar));
}

} // namespace sgui
