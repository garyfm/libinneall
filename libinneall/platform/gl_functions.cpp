#include <EGL/egl.h>
#include <libinneall/base/assert.hpp>
#include <libinneall/platform/gl_functions.hpp>

#define GLFUNC_DEFINE(type, name) type name { nullptr };

INL_FOR_EACH_GLFUNC(GLFUNC_DEFINE);
#undef GLFUNC_DEFINE

void gl_load_functions() {
#define GLFUNC_LOAD(type, name)                                                                                        \
    name = (type)eglGetProcAddress(#name);                                                                             \
    inl_assert(name != nullptr, "Failed to load GL function: " #name);

    INL_FOR_EACH_GLFUNC(GLFUNC_LOAD);
#undef GLFUNC_LOAD
}
