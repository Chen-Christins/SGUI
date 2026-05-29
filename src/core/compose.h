#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "badge.h"
#include "checkbox.h"
#include "chip.h"
#include "context_menu.h"
#include "dialog.h"
#include "dropdown.h"
#include "group_box.h"
#include "image.h"
#include "layout.h"
#include "loading_spinner.h"
#include "modifier.h"
#include "progress_bar.h"
#include "radio_button.h"
#include "remember.h"
#include "scroll.h"
#include "separator.h"
#include "slider.h"
#include "stepper.h"
#include "tabs.h"
#include "text_field.h"
#include "toggle.h"
#include "tooltip.h"
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

inline std::shared_ptr<Widget> spacer() {
    return std::make_shared<Spacer>();
}

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

inline std::shared_ptr<Widget> radioButton(MutableState<int>& selectedId, int id, std::string label, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<RadioButton>(selectedId, id, std::move(label)));
}

inline std::shared_ptr<Widget> dropdown(MutableState<int>& selectedIndex, std::vector<std::string> options, bool& isOpen,
                                        Modifier modifier = Modifier()) {
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

// ---- Overlay widgets (measure=0, ctx unchanged, place at end of tree) ----

inline std::shared_ptr<Widget> dialog(MutableState<bool>& isOpen, std::string title, std::shared_ptr<Widget> content,
                                      std::vector<std::string> buttonLabels, MutableState<int>& buttonClicked,
                                      Modifier modifier = Modifier()) {
    return modifier.then(
        std::make_shared<Dialog>(isOpen, std::move(title), std::move(content), std::move(buttonLabels), buttonClicked));
}

inline std::shared_ptr<Widget> tooltip(bool& showTooltip, std::shared_ptr<Widget> content, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Tooltip>(showTooltip, std::move(content)));
}

inline std::shared_ptr<Widget> contextMenu(MutableState<bool>& isOpen, int& screenX, int& screenY, std::vector<std::string> items,
                                           MutableState<int>& selectedItem, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<ContextMenu>(isOpen, screenX, screenY, std::move(items), selectedItem));
}

// ---- Display widgets ----

inline std::shared_ptr<Widget> badge(int count, std::shared_ptr<Widget> child, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Badge>(count, std::move(child)));
}

inline std::shared_ptr<Widget> chip(std::string label, Color backgroundColor, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Chip>(std::move(label), backgroundColor));
}

inline std::shared_ptr<Widget> groupBox(std::string title, std::shared_ptr<Widget> content, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<GroupBox>(std::move(title), std::move(content)));
}

inline std::shared_ptr<Widget> loadingSpinner(int size = 24, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<LoadingSpinner>(size));
}

inline std::shared_ptr<Widget> stepper(MutableState<int>& value, int min, int max, int step = 1, Modifier modifier = Modifier()) {
    return modifier.then(std::make_shared<Stepper>(value, min, max, step));
}

} // namespace sgui
