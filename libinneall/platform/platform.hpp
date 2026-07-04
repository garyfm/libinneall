#pragma once

#include <libinneall/base/error.hpp>

#include <EGL/egl.h>
#include <xcb/xcb.h>

namespace inl::platform {

struct Window {
    xcb_connection_t* xcb_conn {};
    xcb_window_t xcb_window {};
    int xcb_screen {};
    EGLDisplay display {};
    EGLSurface surface {};
    EGLContext context {};
};

Error window_create(Window& window);
void window_destroy(Window& window);
Error window_map(Window& window);
void window_resize();
float get_time();

void swap_buffers(Window& window);

} // namespace inl::platform
