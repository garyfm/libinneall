#include <libinneal/base/assert.hpp>
#include <libinneal/base/log.hpp>

#include <cstdlib>

namespace inl {

void assert(bool predicate, std::string_view predicate_str, std::string_view msg, std::source_location location) {
    if (predicate) {
        return;
    }

    log::error("Assertion failed ({}:{}): ({}) {}", location.file_name(), location.line(), predicate_str, msg);

    std::abort();
}

} // namespace inl
