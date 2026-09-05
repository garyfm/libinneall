#pragma once

#include <libinneall/base/array.hpp>
#include <libinneall/base/error.hpp>

#include <EGL/egl.h>
#include <time.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

namespace inl::platform {

struct Platform;

enum class InputKeyState : uint8_t {
    Released = 0,
    Pressed,
};

enum class InputKey : uint8_t {
    Unknown = 0,
    w,
    a,
    s,
    d,
    ESC,
};

using CallbackWindowResize = void (*)(Platform&, int32_t, int32_t);
using CallbackInputMousePos = void (*)(Platform&, float, float);

struct Xcb {
    xcb_connection_t* connection {};
    xcb_window_t platform {};
    int screen_num {};
    xcb_atom_t atom_wm_protocols {};
    xcb_atom_t atom_wm_delete {};
    xcb_key_symbols_t* key_symbols {};
    xcb_get_keyboard_mapping_reply_t* keyboard_mapping_reply {};
    xcb_pixmap_t pixmap {};
    xcb_gcontext_t gc {};
    xcb_cursor_t cursor {};
};

struct Egl {
    EGLDisplay display {};
    EGLSurface surface {};
    EGLContext context {};
};

struct Platform {
    Xcb xcb {};
    Egl egl {};
    bool valid {};

    timespec time_start {};

    uint32_t width;
    uint32_t height;
    bool should_exit {};
    CallbackWindowResize callback_window_resize;
    CallbackInputMousePos callback_input_mouse_pos;

    Array<InputKeyState, 256> key_state {};
};

void initialize(Platform& platform);
Error window_create(Platform& platform, uint32_t width, uint32_t height, CallbackWindowResize callback_window_resize,
    CallbackInputMousePos callback_input_mouse_pos);

void window_destroy(Platform& platform);
Error window_map(Platform& platform);
void window_resize(Platform& platform, uint32_t width, uint32_t height);
void window_process_events(Platform& platform);
bool window_should_exit(Platform& platform);

static inline InputKeyState get_input_key_state(Platform& platform, InputKey key) {
    return platform.key_state[static_cast<uint8_t>(key)];
}

void swap_buffers(Platform& platform);

float get_elapsed_time(Platform& platform);

} // namespace inl::platform
