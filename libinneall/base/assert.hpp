#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define inl_assert(predicate, msg) inl::assert((predicate), (#predicate), (msg))

namespace inl {

inline constexpr void assert(bool predicate, char const* predicate_str, char const* msg = "",
    char const* filename = __builtin_FILE(), size_t line = __builtin_LINE()) {
    if (predicate) {
        return;
    }

    printf("FATAL: Assertion Failed (%s:%lu): (%s) %s", filename, line, predicate_str, msg);

    abort();
}

} // namespace inl
