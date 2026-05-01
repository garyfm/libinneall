#include <libinneall/base/result.hpp>

namespace inl {

StringView to_string(inl::Error e) {
    switch (e) {
    case inl::Error::Ok:
        return "OK";
    case inl::Error::Error:
        return "ERROR";
    default:
        return "Unknown";
    }
}
} // namespace inl
