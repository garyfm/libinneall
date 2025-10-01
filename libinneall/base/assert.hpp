#pragma once

#include <source_location>
#include <string_view>

// TODO: libbacktrace is supported yet ???
// Check this and add if possible
#define INL_ASSERT(predicate, msg) inl::assert((predicate), (#predicate), (msg), std::source_location::current())

namespace inl {

void assert(bool predicate, std::string_view predicate_str, std::string_view msg = {},
    std::source_location location = std::source_location::current());

} // namespace inl
