#pragma once

#include <libinneall/base/assert.hpp>

#include <string_view>

namespace inl {

int memcmp(void const* lhs, void const* rhs, size_t size);

inline constexpr size_t strlen(char const* str) {
    INL_ASSERT(str != nullptr, "Invalid pointer");

    char const* end = str;

    while (*end != '\0') {
        ++end;
    }

    return end - str;
}

template <size_t N> inline constexpr size_t strlen(char const (&str)[N]) {
    size_t len { 0 };
    while ((len < N) && (str[len] != '\0')) {
        ++len;
    }

    return len;
}

inline constexpr bool isspace(char c) {
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

struct Cut {
    std::string_view left;
    std::string_view right;
    bool success;
};

std::string_view trim_left(std::string_view sv);
std::string_view trim_right(std::string_view sv);
std::string_view trim(std::string_view sv);

Cut cut(std::string_view sv, char deliminator);

} // namespace
