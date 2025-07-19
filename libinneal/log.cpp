#include <libinneal/log.hpp>

#include <print>
#include <string_view>

namespace inl {

void log(std::string_view msg) { std::println("{}", msg); }

} // namespace inl
