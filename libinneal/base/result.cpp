#include <libinneal/base/result.hpp>

namespace inl {

std::string_view to_string(inl::Error e) {
    switch (e) {
    case inl::Error::OK:
        return "OK";
    case inl::Error::ERROR:
        return "ERROR";
    default:
        return "Unknown";
    }
}
} // namespace inl
