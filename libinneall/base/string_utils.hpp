#pragma once

#include <libinneall/base/assert.hpp>

#include <stddef.h>
#include <stdint.h>

namespace inl {

int memcmp(void const* lhs, void const* rhs, size_t size);
void memcpy(void* dst, void const* src, size_t size);

inline constexpr size_t strlen(char const* str) {

    INL_ASSERT(str != nullptr, "Invalid string pointer");

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

inline constexpr bool isdigit(char c) { return (c >= '0' && c <= '9'); }

} // namespace inl
