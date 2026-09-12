#pragma once

#include <libinneall/base/array.hpp>
#include <libinneall/base/error.hpp>

#include <EGL/egl.h>
#include <sys/mman.h>
#include <time.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

namespace inl::platform {

struct Platform;

enum class InputKeyState : uint8_t {
    Released = 0,
    Pressed = 1,
};

enum class InputKey : uint8_t {
    Unknown = 0,
    w,
    a,
    s,
    d,
    ESC,
};

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
    CallbackInputMousePos callback_input_mouse_pos;

    Array<InputKeyState, 256> key_state {};
};

Error create(Platform& platform, StringView tite, uint32_t width, uint32_t height,
    CallbackInputMousePos callback_input_mouse_pos);
void destroy(Platform& platform);

Error window_map(Platform& platform);
void window_resize(Platform& platform, uint32_t width, uint32_t height);
void window_process_events(Platform& platform);
bool window_should_exit(Platform& platform);

Error gfx_init(Platform& platform);
void gfx_swap_buffers(Platform& platform);

static inline InputKeyState input_key_state(Platform& platform, InputKey key) {
    return platform.key_state[static_cast<uint8_t>(key)];
}

static inline float aspect_ratio(Platform& platform) {
    return static_cast<float>(platform.width) / static_cast<float>(platform.height);
};

float elapsed_time(Platform& platform);

void* mem_alloc(size_t size);
void mem_free(void* mem, size_t size);

} // namespace inl::platform
