#include <libinneall/base/log.hpp>
#include <libinneall/platform/platform.hpp>

#include <EGL/eglext.h>
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <X11/keysym.h>
#include <unistd.h>

namespace {

constexpr uint32_t XCB_RESPOSE_TYPE_MASK = ~0x80;

}

namespace inl::platform {

static Error xcb_connection_create(Window& window, uint16_t width, uint16_t height) {
    // TODO:: Review based on I3
    // Connect
    int screen_num;
    window.xcb_conn = xcb_connect(nullptr, &screen_num);

    if (window.xcb_conn == nullptr || xcb_connection_has_error(window.xcb_conn)) {
        return Error::PlatformFailedToConnectToWindowServer;
    }

    // Get screen
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(window.xcb_conn));
    for (int i = 0; i < screen_num; ++i) {

        xcb_screen_next(&screen_iter);
    }

    xcb_screen_t* screen = screen_iter.data;

    // Create Window
    {
        window.xcb_window = xcb_generate_id(window.xcb_conn);
        uint32_t window_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

        uint32_t window_values[] = {
            0,
            XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE
                | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW
                | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_STRUCTURE_NOTIFY,
        };

        xcb_void_cookie_t cookie
            = xcb_create_window_checked(window.xcb_conn, XCB_COPY_FROM_PARENT, window.xcb_window, screen->root, 0, 0,
                width, height, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, window_mask, window_values);

        xcb_generic_error_t* error = xcb_request_check(window.xcb_conn, cookie);

        /// TODO defer free here
        if (error != NULL) {
            log_error("XCB error: %x", error->error_code);
            free(error);
            return Error::PlatformFailedToCreateWindow;
        }

        free(error);

        // Set the window name
        xcb_change_property(window.xcb_conn, XCB_PROP_MODE_REPLACE, window.xcb_window, XCB_ATOM_WM_NAME,
            XCB_ATOM_STRING, 8, sizeof("OpenGL Window") - 1, "OpenGL Window");
    }

    // Replace default window delete function
    {

        xcb_intern_atom_cookie_t atom_cookie_protocols = xcb_intern_atom(window.xcb_conn, 1, 12, "WM_PROTOCOLS");
        xcb_intern_atom_cookie_t atom_cookie_delete = xcb_intern_atom(window.xcb_conn, 0, 16, "WM_DELETE_WINDOW");
        xcb_intern_atom_reply_t* atom_reply_wm_protocols
            = xcb_intern_atom_reply(window.xcb_conn, atom_cookie_protocols, 0);
        xcb_intern_atom_reply_t* atom_reply_delete_window
            = xcb_intern_atom_reply(window.xcb_conn, atom_cookie_delete, 0);
        xcb_change_property(window.xcb_conn, XCB_PROP_MODE_REPLACE, window.xcb_window, atom_reply_wm_protocols->atom,
            XCB_ATOM_ATOM, 32, 1, &atom_reply_delete_window->atom);
        window.xcb_atom_reply_wm_protocols = atom_reply_wm_protocols;
        window.xcb_atom_delete_window = atom_reply_delete_window;
    }

    // Key Symbols
    {
        window.xcb_key_symbols = xcb_key_symbols_alloc(window.xcb_conn);
        xcb_setup_t const* setup = xcb_get_setup(window.xcb_conn);

        xcb_get_keyboard_mapping_cookie_t cookie = xcb_get_keyboard_mapping(
            window.xcb_conn, setup->min_keycode, (uint8_t)(setup->max_keycode - setup->min_keycode) + 1);
        xcb_generic_error_t* error {};
        window.xcb_keyboard_mapping_reply = xcb_get_keyboard_mapping_reply(window.xcb_conn, cookie, &error);
        // TODO: handle error
    }
    return Error::Ok;
}

static Error egl_ctx_create(Window& window) {

    // Configure EGL to use XCB
    static EGLAttrib const platform_attribute_list[] = {
        EGL_PLATFORM_XCB_SCREEN_EXT,
        window.xcb_screen,
        EGL_NONE,
    };

    window.display = eglGetPlatformDisplay(EGL_PLATFORM_XCB_EXT, (void*)window.xcb_conn, platform_attribute_list);

    if (window.display == EGL_NO_DISPLAY) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformGfxFailedToGetDisplay;
    }

    EGLint major { 0 };
    EGLint minor { 0 };
    EGLBoolean success = eglInitialize(window.display, &major, &minor);
    if (!success) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformGfxFailedToInit;
    }

    if (major != 1 || (major == 1 && minor < 5)) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformGfxUnsupportedVersion;
    }

    success = eglBindAPI(EGL_OPENGL_API);
    if (!success) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformGfxFailedToBind;
    }

    // Configure EGL frambuffer
    EGLint attr[] = {
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT,
        EGL_CONFORMANT,
        EGL_OPENGL_BIT,
        EGL_RENDERABLE_TYPE,
        EGL_OPENGL_BIT,
        EGL_COLOR_BUFFER_TYPE,
        EGL_RGB_BUFFER,
        EGL_RED_SIZE,
        8,
        EGL_GREEN_SIZE,
        8,
        EGL_BLUE_SIZE,
        8,
        EGL_DEPTH_SIZE,
        24,
        EGL_STENCIL_SIZE,
        8,
        EGL_NONE,
    };

    EGLConfig config;
    EGLint num_config;
    success = eglChooseConfig(window.display, attr, &config, 1, &num_config);
    if (!success || num_config != 1) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformGfxFailedToConfigure;
    }

    // Create Window surface
    EGLAttrib window_attr[] = {
        EGL_GL_COLORSPACE,
        EGL_GL_COLORSPACE_LINEAR, // or use EGL_GL_COLORSPACE_SRGB for sRGB framebuffer
        EGL_RENDER_BUFFER,
        EGL_BACK_BUFFER,
        EGL_NONE,
    };

    window.surface = eglCreatePlatformWindowSurface(window.display, config, (void*)&window.xcb_window, window_attr);
    if (window.surface == EGL_NO_SURFACE) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformGfxFailedToBind;
    }

    // Create OpenGL contex
    EGLint context_attr[] = {
        EGL_CONTEXT_MAJOR_VERSION,
        4,
        EGL_CONTEXT_MINOR_VERSION,
        5,
        EGL_CONTEXT_OPENGL_PROFILE_MASK,
        EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
        EGL_CONTEXT_OPENGL_DEBUG,
        EGL_TRUE,
        EGL_NONE,
    };

    window.context = eglCreateContext(window.display, config, EGL_NO_CONTEXT, context_attr);
    if (window.context == EGL_NO_CONTEXT) {
        log_error("EGL Error: %x", eglGetError());
        return Error::PlatformGfxFailedToCreateCtx;
    }

    success = eglMakeCurrent(window.display, window.surface, window.surface, window.context);
    if (!success) {
        log_error("EGL Error: %x", eglGetError());
        return Error::PlatformGfxFailedToMakeCurrent;
    }

    return Error::Ok;
}

void initialize(Window& window) { clock_gettime(CLOCK_MONOTONIC, &window.time_start); }

Error window_create(Window& window, uint32_t width, uint32_t height, CallbackWindowResize callback_window_resize,
    CallbackInputKey callback_input_key, CallbackInputMousePos callback_input_mouse_pos) {

    TRY(xcb_connection_create(window, static_cast<uint16_t>(width), static_cast<uint16_t>(height)));
    TRY(egl_ctx_create(window));

    window.width = width;
    window.height = height;
    window.callback_window_resize = callback_window_resize;
    window.callback_input_key = callback_input_key;
    window.callback_input_mouse_pos = callback_input_mouse_pos;

    return Error::Ok;
}

void window_destroy(Window& window) {
    eglMakeCurrent(window.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglDestroyContext(window.display, window.context);
    eglDestroySurface(window.display, window.surface);
    eglTerminate(window.display);

    free(window.xcb_atom_reply_wm_protocols);
    free(window.xcb_atom_delete_window);

    xcb_destroy_window(window.xcb_conn, window.xcb_window);
    xcb_disconnect(window.xcb_conn);
}

Error window_map(Window& window) {
    inl_assert(window.xcb_conn != nullptr, "Invalid window");

    xcb_map_window(window.xcb_conn, window.xcb_window);
    xcb_flush(window.xcb_conn);

    return Error::Ok;
}

void window_process_events(Window& window) {
    xcb_generic_event_t* xcb_event {};

    while ((xcb_event = xcb_poll_for_event(window.xcb_conn))) {

        switch (xcb_event->response_type & XCB_RESPOSE_TYPE_MASK) {
        case XCB_EXPOSE: {
            log_debug("window event: XCB_EXPOSE");
            break;
        }
        case XCB_CONFIGURE_NOTIFY: {
            xcb_configure_notify_event_t* configure_event = (xcb_configure_notify_event_t*)xcb_event;
            window.width = configure_event->width;
            window.height = configure_event->height;
            window.callback_window_resize(window, configure_event->width, configure_event->height);
            break;
        }
        case XCB_KEY_PRESS:
        case XCB_KEY_RELEASE: {
            xcb_key_press_event_t* key_event = (xcb_key_press_event_t*)xcb_event;

            xcb_setup_t const* setup = xcb_get_setup(window.xcb_conn);
            xcb_keysym_t* keysyms = xcb_get_keyboard_mapping_keysyms(window.xcb_keyboard_mapping_reply);

            keysyms = &keysyms[(key_event->detail - setup->min_keycode)
                * window.xcb_keyboard_mapping_reply->keysyms_per_keycode];
            // TODO: col is to handle mode switch. Not impelemented yet.
            xcb_keysym_t keysym = keysyms[0];

            InputKey key {};
            switch (keysym) {
            case XK_w:
                key = InputKey::w;
                break;
            case XK_a:
                key = InputKey::a;
                break;
            case XK_s:
                key = InputKey::s;
                break;
            case XK_d:
                key = InputKey::d;
                break;
            default:
                break;
            }

            InputKeyAction action = InputKeyAction::Pressed;
            if ((key_event->response_type & XCB_RESPOSE_TYPE_MASK) == XCB_KEY_RELEASE) {
                action = InputKeyAction::Released;
            }
            window.callback_input_key(window, key, action);
            break;
        }
        case XCB_BUTTON_PRESS: {
            break;
        }
        case XCB_BUTTON_RELEASE: {
            break;
        }
        case XCB_MOTION_NOTIFY: {
            xcb_motion_notify_event_t* motion_event = (xcb_motion_notify_event_t*)xcb_event;
            window.callback_input_mouse_pos(window, motion_event->root_x, motion_event->root_y);
            break;
        }
        case XCB_ENTER_NOTIFY: {
            break;
        }
        case XCB_CLIENT_MESSAGE: {
            if ((*(xcb_client_message_event_t*)xcb_event).data.data32[0] == window.xcb_atom_delete_window->atom) {
                window.should_exit = true;
                break;
            }
            break;
        }
        default: {
            break;
        }
        }
        free(xcb_event);
    }
}

void window_resize(Window& window, uint32_t width, uint32_t height) {
    inl_assert(window.xcb_conn != nullptr, "Invalid window");

    const static uint32_t values[] = { width, height };
    window.width = width;
    window.height = height;

    xcb_configure_window(
        window.xcb_conn, window.xcb_window, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
    xcb_flush(window.xcb_conn);
}

bool window_should_exit(Window& window) { return window.should_exit; }

void swap_buffers(Window& window) {
    inl_assert(window.xcb_conn != nullptr, "Invalid window");
    eglSwapBuffers(window.display, window.surface);
}

float get_elapsed_time(Window& window) {
    timespec now {};
    clock_gettime(CLOCK_MONOTONIC, &now);

    timespec td {
        now.tv_sec - window.time_start.tv_sec,
        now.tv_nsec - window.time_start.tv_nsec,
    };

    static constexpr uint32_t NS_PER_SEC = 1000000000;
    float elapsed = float(td.tv_sec) + (float(td.tv_nsec) / NS_PER_SEC);

    return elapsed;
}

} // namespace inl::platform
