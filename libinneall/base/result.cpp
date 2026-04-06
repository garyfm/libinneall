#include <libinneall/base/result.hpp>

namespace inl {

StringView to_string(inl::Error e) {
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
