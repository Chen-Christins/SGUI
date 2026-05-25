#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "checkbox.h"
#include "dropdown.h"
#include "image.h"
#include "layout.h"
#include "modifier.h"
#include "progress_bar.h"
#include "radio_button.h"
#include "remember.h"
#include "scroll.h"
#include "separator.h"
#include "slider.h"
#include "tabs.h"
#include "text_field.h"
#include "toggle.h"
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
                                         CompositionContext& ctx, std::string placeholder = "", Modifier modifier = Modifier(),
                                         int minTabId = -1, int maxTabId = -1) {
    auto& cursorPos = ctx.remember<int>([]() { return 0; });
    return modifier.then(std::make_shared<TextField>(focusedId, id, textState, cursorPos, std::move(placeholder), minTabId, maxTabId));
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

inline std::shared_ptr<Widget> slider(MutableState<float>& valueState, float min, float max, float step = 0.0f,
                                       Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Slider>(valueState, min, max, step));
}

inline std::shared_ptr<Widget> toggle(MutableState<bool>& onState, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Toggle>(onState));
}

inline std::shared_ptr<Widget> imageView(Texture2D texture, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<ImageView>(texture));
}

inline std::shared_ptr<Widget> imageView(Texture2D texture, int width, int height, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<ImageView>(texture, width, height));
}

inline std::shared_ptr<Widget> progressBar(MutableState<float>& progress, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<ProgressBar>(progress));
}

inline std::shared_ptr<Widget> separator(bool vertical = false, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Separator>(vertical));
}

inline std::shared_ptr<Widget> radioButton(MutableState<int>& selectedId, int id, std::string label,
                                            Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<RadioButton>(selectedId, id, std::move(label)));
}

inline std::shared_ptr<Widget> dropdown(MutableState<int>& selectedIndex, std::vector<std::string> options,
                                         bool& isOpen, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Dropdown>(selectedIndex, std::move(options), isOpen));
}

inline std::shared_ptr<Widget> tabBar(MutableState<int>& selectedTab, std::vector<std::string> labels,
                                       Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<TabBar>(selectedTab, std::move(labels)));
}

inline std::shared_ptr<Widget> tabPanel(int index, MutableState<int>& selectedTab, std::shared_ptr<Widget> content,
                                         Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<TabPanel>(index, selectedTab, std::move(content)));
}

} // namespace sgui
