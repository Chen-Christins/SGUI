/**
 * @file modifier.hpp
 * @brief 组件修饰器
 * @author Christins (chen.christins@qq.com)
 * @copyright Apache 2.0
 */
#pragma once

namespace sgui {

class Modifier {
public:
    // 链式调用设定 Modifier
    Modifier& fillMaxWidth(bool fill = true) {
        fillMaxWidth_ = fill;
        return *this;
    }

    Modifier& fillMaxHeight(bool fill = true) {
        fillMaxHeight_ = fill;
        return *this;
    }

    Modifier& fillMaxSize(bool fill = true) {
        fillMaxWidth_ = fill;
        fillMaxHeight_ = fill;
        return *this;
    }

    bool fillMaxWidth_ = false;
    bool fillMaxHeight_ = false;
};

} // namespace sgui