/**
 * @file nocopyable.hpp
 * @brief 禁止拷贝和赋值的基类
 * @author Christins (chen.christins@qq.com)
 * @date 2026-05-30
 * @copyright Apache 2.0
 */
#pragma once

namespace sgui {

class NoCopyable {
public:
    NoCopyable() = default;
    NoCopyable(const NoCopyable&) = delete;
    NoCopyable& operator=(const NoCopyable&) = delete;
};

} // namespace sgui