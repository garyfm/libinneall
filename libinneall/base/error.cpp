#include <libinneall/base/error.hpp>

namespace inl {

#define AS_STRING(X) ("Error::" #X),
static char const* error_strings[] = { INL_FOR_EACH_ERROR(AS_STRING) };
#undef AS_STRING

StringView to_string(inl::Error e) { return error_strings[static_cast<uint32_t>(e)]; }

} // namespace inl
