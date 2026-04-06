#pragma once

#include <print>
#include <source_location>
#include <stddef.h>
#include <stdlib.h>

#define INL_ASSERT(predicate, msg) inl::assert((predicate), (#predicate), (msg), std::source_location::current())

namespace inl {

template <size_t PN, size_t MN>
inline constexpr void assert(bool predicate, char const (&predicate_str)[PN], char const (&msg)[MN] = {},
    std::source_location location = std::source_location::current()) {
    if (predicate) {
        return;
    }

    std::println("FATAL: Assertion failed ({}:{}): ({}) {}", location.file_name(), location.line(), predicate_str, msg);

    std::abort();
}

} // namespace inl
