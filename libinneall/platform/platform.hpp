#pragma once

#include <libinneall/base/error.hpp>

#include <EGL/egl.h>
#include <time.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

namespace inl::platform {

struct Window;

enum class WindowEventKind {
    None,
    Exit,
    Resize,
};

enum class InputKeyAction {
    Pressed,
    Released,
};

enum class InputKey {
    Unknown = 0,
    w,
    a,
    s,
    d,
};

using CallbackWindowResize = void (*)(Window&, int32_t, int32_t);
using CallbackInputKey = void (*)(Window&, InputKey key, InputKeyAction action);
using CallbackInputMousePos = void (*)(Window&, float, float);

struct Window {
    // TODO: Only define for linux ? Or hide it in impelementation ?
    xcb_connection_t* xcb_conn {};
    xcb_window_t xcb_window {};
    int xcb_screen {};
    xcb_intern_atom_reply_t* xcb_atom_reply_wm_protocols {};
    xcb_intern_atom_reply_t* xcb_atom_delete_window {};
    xcb_key_symbols_t* xcb_key_symbols {};
    xcb_get_keyboard_mapping_reply_t* xcb_keyboard_mapping_reply {};
    EGLDisplay display {};
    EGLSurface surface {};
    EGLContext context {};

    timespec time_start {};

    uint32_t width;
    uint32_t height;
    bool should_exit {};
    CallbackWindowResize callback_window_resize;
    CallbackInputKey callback_input_key;
    CallbackInputMousePos callback_input_mouse_pos;

    float prev_cursor_x {};
    float prev_cursor_y {};
    float virt_cursor_x {};
    float virt_cursor_y {};
};

struct WindowEventResize {
    size_t width;
    size_t height;
};

struct WindowEvent {
    WindowEventKind kind;

    union {
        WindowEventResize resize;
    };
};

void initialize(Window& window);
Error window_create(Window& window, uint32_t width, uint32_t height, CallbackWindowResize callback_window_resize,
    CallbackInputKey callback_input_key, CallbackInputMousePos callback_input_mouse_pos);

void window_destroy(Window& window);
Error window_map(Window& window);
void window_resize(Window& window, uint32_t width, uint32_t height);
void window_process_events(Window& window);
bool window_should_exit(Window& window);

void swap_buffers(Window& window);

float get_elapsed_time(Window& window);

} // namespace inl::platform
