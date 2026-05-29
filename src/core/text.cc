#include "text.hpp"

namespace sgui {

Text::Text(const char* text) 
    : Text(std::string(text)) {
}

Text::Text(const std::string& text) 
    : text_(text) {
}

void Text::render(RenderContext& ctx) {
    
}

Size Text::measure() const {
    
    return {};
}

} // namespace sgui