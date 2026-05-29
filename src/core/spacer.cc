#include "spacer.hpp"

namespace sgui {

Spacer::Spacer(int width, int height)
    : width_(width), height_(height) {
}

void Spacer::render(RenderContext& ctx) {
    // Spacer 不需要渲染任何东西，占位即可
}

Size Spacer::measure() const {
    return {width_, height_};
}

} // namespace sgui