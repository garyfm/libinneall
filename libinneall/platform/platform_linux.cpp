#include <libinneall/base/defer.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/platform/gl_functions.hpp>
#include <libinneall/platform/platform.hpp>

#include <EGL/eglext.h>
#include <X11/keysym.h>
#include <unistd.h>
#include <xcb/xinput.h>
#define explicit explicit_
#include <xcb/xkb.h>
#undef explicit

namespace {

constexpr uint32_t XCB_RESPOSE_TYPE_MASK = ~0x80;

}

namespace inl::platform {

static Error x_create_cursor(Xcb& xcb) {

    { // Create a drawable pixmap
        xcb.pixmap = xcb_generate_id(xcb.connection);
        xcb_void_cookie_t cookie = xcb_create_pixmap_checked(xcb.connection, 1, xcb.pixmap, xcb.platform, 1, 1);
        xcb_generic_error_t* error = xcb_request_check(xcb.connection, cookie);
        inl_defer(free(error));
        if (error) return Error::PlatformXcbError;
    }

    { // Create a graphic context using the drawable pixmap
        xcb.gc = xcb_generate_id(xcb.connection);
        xcb_void_cookie_t cookie = xcb_create_gc_checked(xcb.connection, xcb.gc, xcb.pixmap, 0, nullptr);
        xcb_generic_error_t* error = xcb_request_check(xcb.connection, cookie);
        inl_defer(free(error));
        if (error) return Error::PlatformXcbError;
    }

    // Fill the pixmap with 0's
    xcb_rectangle_t rect = { 0, 0, 1, 1 };
    xcb_poly_fill_rectangle(xcb.connection, xcb.pixmap, xcb.gc, 1, &rect);

    // Create the curosr as the pixmap
    xcb.cursor = xcb_generate_id(xcb.connection);
    xcb_create_cursor(xcb.connection, xcb.cursor, xcb.pixmap, xcb.pixmap, 0, 0, 0, 0, 0, 0, 0, 0);
    return Error::Ok;
}

static Error x_create(Xcb& xcb, uint16_t width, uint16_t height, StringView window_title) {
    // Connect
    xcb.connection = xcb_connect(nullptr, &xcb.screen_num);

    if (xcb.connection == nullptr || xcb_connection_has_error(xcb.connection)) {
        return Error::PlatformXcbError;
    }

    // Get screen
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(xcb.connection));
    for (int i = 0; i < xcb.screen_num; ++i) {
        xcb_screen_next(&screen_iter);
    }

    xcb_screen_t* screen = screen_iter.data;

    { // Create Platform
        xcb.platform = xcb_generate_id(xcb.connection);
        uint32_t window_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

        uint32_t window_values[] = {
            0,
            XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_ENTER_WINDOW
                | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION
                | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW | XCB_EVENT_MASK_KEY_PRESS
                | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_STRUCTURE_NOTIFY,
        };

        xcb_void_cookie_t cookie
            = xcb_create_window_checked(xcb.connection, XCB_COPY_FROM_PARENT, xcb.platform, screen->root, 0, 0, width,
                height, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, window_mask, window_values);

        xcb_generic_error_t* error = xcb_request_check(xcb.connection, cookie);
        inl_defer(free(error));

        if (error != NULL) return Error::PlatformXcbError;

        // Set the platform name
        xcb_change_property(xcb.connection, XCB_PROP_MODE_REPLACE, xcb.platform, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
            static_cast<uint32_t>(window_title.size()), window_title.data());
    }

    { // Replace default platform delete function

        xcb_intern_atom_cookie_t atom_cookie_wm_protocols = xcb_intern_atom(xcb.connection, 1, 12, "WM_PROTOCOLS");
        xcb_intern_atom_reply_t* atom_reply_wm_protocols
            = xcb_intern_atom_reply(xcb.connection, atom_cookie_wm_protocols, 0);
        inl_defer(free(atom_reply_wm_protocols););
        if (!atom_reply_wm_protocols) return Error::PlatformXcbError;

        xcb.atom_wm_protocols = atom_reply_wm_protocols->atom;

        xcb_intern_atom_cookie_t atom_cookie_wm_delete = xcb_intern_atom(xcb.connection, 0, 16, "WM_DELETE_WINDOW");
        xcb_intern_atom_reply_t* atom_reply_delete_window
            = xcb_intern_atom_reply(xcb.connection, atom_cookie_wm_delete, 0);
        inl_defer(free(atom_reply_delete_window););

        if (!atom_reply_delete_window) return Error::PlatformXcbError;

        xcb.atom_wm_delete = atom_reply_delete_window->atom;

        xcb_void_cookie_t cookie = xcb_change_property_checked(xcb.connection, XCB_PROP_MODE_REPLACE, xcb.platform,
            atom_reply_wm_protocols->atom, XCB_ATOM_ATOM, 32, 1, &atom_reply_delete_window->atom);
        xcb_generic_error_t* error = xcb_request_check(xcb.connection, cookie);
        inl_defer(free(error));
        if (error) return Error::PlatformXcbError;
    }

    { // Key Symbols
        xcb.key_symbols = xcb_key_symbols_alloc(xcb.connection);
        xcb_setup_t const* setup = xcb_get_setup(xcb.connection);

        xcb_get_keyboard_mapping_cookie_t cookie = xcb_get_keyboard_mapping(
            xcb.connection, setup->min_keycode, (uint8_t)(setup->max_keycode - setup->min_keycode) + 1);
        xcb_generic_error_t* error {};
        xcb.keyboard_mapping_reply = xcb_get_keyboard_mapping_reply(xcb.connection, cookie, &error);
        inl_defer(free(error));
        if (error) return Error::PlatformXcbError;
    }

    { // Check XInput2 version

        xcb_input_xi_query_version_cookie_t cookie = xcb_input_xi_query_version(xcb.connection, 2, 3);

        xcb_generic_error_t* error = NULL;

        xcb_input_xi_query_version_reply_t* reply = xcb_input_xi_query_version_reply(xcb.connection, cookie, &error);
        inl_defer({
            free(error);
            free(reply);
        });

        if (!reply) return Error::PlatformXcbError;

        log_debug("XInput2 version %u.%u\n", reply->major_version, reply->minor_version);
        if (reply->major_version < 2 || (reply->major_version >= 2 && reply->minor_version < 3)) {
            return Error::PlatformXcbError;
        }
    }

    { // Register for XInput2 Events. Used for RAW mouse input
        // NOTE: For some reason xcb defines xcb_input_event_mask_t with out the
        // mask array x111 deinfes it as typedef struct
        //{
        //    int                 deviceid;
        //    int                 mask_len;
        //    unsigned char*      mask;
        //} XIEventMask;
        // The XCB api still expects the same memory layout so I've wrapped it in a
        // struct and added the mask array

        struct EventMask {
            xcb_input_event_mask_t header;
            xcb_input_xi_event_mask_t mask;
        };

        EventMask event_mask;
        event_mask.header.deviceid = XCB_INPUT_DEVICE_ALL;
        event_mask.header.mask_len = 1;
        event_mask.mask = XCB_INPUT_XI_EVENT_MASK_RAW_MOTION;

        xcb_void_cookie_t cookie
            = xcb_input_xi_select_events_checked(xcb.connection, screen->root, 1, (xcb_input_event_mask_t*)&event_mask);
        xcb_generic_error_t* error = xcb_request_check(xcb.connection, cookie);
        inl_defer(free(error));

        if (error) return Error::PlatformXcbError;
    }

    { // Setup XKB
        xcb_xkb_use_extension_cookie_t cookie
            = xcb_xkb_use_extension(xcb.connection, XCB_XKB_MAJOR_VERSION, XCB_XKB_MINOR_VERSION);

        xcb_generic_error_t* error {};
        xcb_xkb_use_extension_reply_t* reply = xcb_xkb_use_extension_reply(xcb.connection, cookie, &error);
        inl_defer({
            free(error);
            free(reply);
        });

        if (!reply || error) return Error::PlatformXcbError;
    }

    { // Enable auto repeat on keys
        xcb_generic_error_t* error {};

        auto repeat_cookie = xcb_xkb_per_client_flags(xcb.connection, XCB_XKB_ID_USE_CORE_KBD,
            XCB_XKB_PER_CLIENT_FLAG_DETECTABLE_AUTO_REPEAT, XCB_XKB_PER_CLIENT_FLAG_DETECTABLE_AUTO_REPEAT, 0, 0, 0);

        xcb_xkb_per_client_flags_reply_t* reply = xcb_xkb_per_client_flags_reply(xcb.connection, repeat_cookie, &error);
        inl_defer({
            free(error);
            free(reply);
        });
        if (!reply || error) return Error::PlatformXcbError;
    }

    TRY(x_create_cursor(xcb));

    return Error::Ok;
}

static void x_destroy(Xcb& xcb) {
    free(xcb.key_symbols);
    free(xcb.keyboard_mapping_reply);
    xcb_free_gc(xcb.connection, xcb.gc);
    xcb_free_pixmap(xcb.connection, xcb.pixmap);
    xcb_free_cursor(xcb.connection, xcb.cursor);

    xcb_destroy_window(xcb.connection, xcb.platform);
    xcb_disconnect(xcb.connection);
}

static xcb_keysym_t x_get_keysym(Xcb& xcb, xcb_key_press_event_t& key_event) {
    xcb_setup_t const* setup = xcb_get_setup(xcb.connection);
    xcb_keysym_t* keysyms = xcb_get_keyboard_mapping_keysyms(xcb.keyboard_mapping_reply);

    keysyms = &keysyms[(key_event.detail - setup->min_keycode) * xcb.keyboard_mapping_reply->keysyms_per_keycode];
    // TODO: col is to handle mode switch. Not impelemented yet.
    xcb_keysym_t keysym = keysyms[0];

    return keysym;
}

static Error x_grab_cursor(Xcb& xcb) {

    xcb_grab_pointer_cookie_t cookie = xcb_grab_pointer(xcb.connection, 1, xcb.platform, XCB_EVENT_MASK_POINTER_MOTION,
        XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, xcb.platform, xcb.cursor, XCB_CURRENT_TIME);
    xcb_generic_error_t* error {};
    xcb_grab_pointer_reply_t* reply = xcb_grab_pointer_reply(xcb.connection, cookie, &error);
    inl_defer({
        free(error);
        free(reply);
    });

    if (!reply || error) return Error::PlatformXcbError;

    xcb_flush(xcb.connection);
    return Error::Ok;
}

static Error x_ungrab_cursor(Xcb& xcb) {

    xcb_void_cookie_t cookie = xcb_ungrab_pointer_checked(xcb.connection, XCB_CURRENT_TIME);
    xcb_generic_error_t* error = xcb_request_check(xcb.connection, cookie);
    inl_defer({ free(error); });

    if (error) return Error::PlatformXcbError;
    xcb_flush(xcb.connection);
    return Error::Ok;
}
static Error egl_create(Egl& egl, Xcb& xcb) {

    // Configure EGL to use XCB
    static EGLAttrib const platform_attribute_list[] = {
        EGL_PLATFORM_XCB_SCREEN_EXT,
        xcb.screen_num,
        EGL_NONE,
    };

    egl.display = eglGetPlatformDisplay(EGL_PLATFORM_XCB_EXT, (void*)xcb.connection, platform_attribute_list);

    if (egl.display == EGL_NO_DISPLAY) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformGfxFailedToGetDisplay;
    }

    EGLint major { 0 };
    EGLint minor { 0 };
    EGLBoolean success = eglInitialize(egl.display, &major, &minor);
    if (!success) {
        return Error::PlatformEglError;
    }

    if (major != 1 || (major == 1 && minor < 5)) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformEglError;
    }

    success = eglBindAPI(EGL_OPENGL_API);
    if (!success) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformEglError;
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
    success = eglChooseConfig(egl.display, attr, &config, 1, &num_config);
    if (!success || num_config != 1) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformEglError;
    }

    // Create Window surface
    EGLAttrib window_attr[] = {
        EGL_GL_COLORSPACE,
        EGL_GL_COLORSPACE_LINEAR, // or use EGL_GL_COLORSPACE_SRGB for sRGB
                                  // framebuffer
        EGL_RENDER_BUFFER,
        EGL_BACK_BUFFER,
        EGL_NONE,
    };

    egl.surface = eglCreatePlatformWindowSurface(egl.display, config, (void*)&xcb.platform, window_attr);
    if (egl.surface == EGL_NO_SURFACE) {
        log_error("EGL error: %x", eglGetError());
        return Error::PlatformEglError;
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

    egl.context = eglCreateContext(egl.display, config, EGL_NO_CONTEXT, context_attr);
    if (egl.context == EGL_NO_CONTEXT) {
        log_error("EGL Error: %x", eglGetError());
        return Error::PlatformGfxFailedToCreateCtx;
    }

    success = eglMakeCurrent(egl.display, egl.surface, egl.surface, egl.context);
    if (!success) {
        log_error("EGL Error: %x", eglGetError());
        return Error::PlatformEglError;
    }

    return Error::Ok;
}

static void egl_destroy(Egl& egl) {
    eglMakeCurrent(egl.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglDestroyContext(egl.display, egl.context);
    eglDestroySurface(egl.display, egl.surface);
    eglTerminate(egl.display);
}

static void window_handle_key_input(Platform& platform, xcb_key_press_event_t& event) {
    xcb_keysym_t keysym = x_get_keysym(platform.xcb, event);

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
    case XK_Escape:
        platform.should_exit = true;
        break;

    default:
        break;
    }

    if ((event.response_type & XCB_RESPOSE_TYPE_MASK) == XCB_KEY_PRESS) {
        platform.key_state[static_cast<uint8_t>(key)] = InputKeyState::Pressed;
    } else {
        platform.key_state[static_cast<uint8_t>(key)] = InputKeyState::Released;
    }
}

static void window_handle_mouse_input(Platform& platform, xcb_input_raw_motion_event_t& event) {
    const xcb_input_fp3232_t* v = xcb_input_raw_button_press_axisvalues(&event);
    double dx = v[0].integral + v[0].frac / 4294967296.0;
    double dy = v[1].integral + v[1].frac / 4294967296.0;
    platform.callback_input_mouse_pos(platform, (float)dx, (float)dy);
}

void APIENTRY opengl_debug_callback(GLenum source, GLenum type, uint32_t id, GLenum severity,
    [[maybe_unused]] GLsizei length, const char* message, [[maybe_unused]] const void* userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    inl::StringView source_str {};
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        source_str = "api";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        source_str = "window system";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        source_str = "shader compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        source_str = "third party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        source_str = "application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        source_str = "other";
        break;
    default:
        source_str = "unknown";
    }

    inl::StringView type_str {};
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        type_str = "error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_str = "deprecated behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_str = "undefined behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type_str = "portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type_str = "performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        type_str = "marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        type_str = "push group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        type_str = "pop group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        type_str = "other";
        break;
    default:
        type_str = "unknown";
    }

    inl::StringView severity_str {};
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        severity_str = "high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severity_str = "medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        severity_str = "low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        severity_str = "notification";
        break;
    default:
        severity_str = "unknown";
    }

    log_info("OpenGL debug callback - servierty: %s source: %s type: %s msg: %s ", severity_str, source_str, type_str,
        message);

    if (type == GL_DEBUG_TYPE_ERROR) {
        log_error("OpenGL Debug Error... Abort");
        abort();
    }
}

static void gfx_resize([[maybe_unused]] inl::platform::Platform& platform, int width, int height) {
    // NOTE: x11 will handle resizing the window if the user changes its size so only update the opengl view port
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void gfx_swap_buffers(Platform& platform) {
    inl_assert(platform.valid, "Invalid platform");
    eglSwapBuffers(platform.egl.display, platform.egl.surface);
}

Error gfx_init(Platform& platform) {
    gl_load_functions();

    glViewport(0, 0, static_cast<GLsizei>(platform.width), static_cast<GLsizei>(platform.height));

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_debug_callback, nullptr);

    // NOTE: This can be used to filter opengl debug messages
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    log_debug("GL: %d %d %d %d\n", vp[0], vp[1], vp[2], vp[3]);

    return Error::Ok;
}

Error create(Platform& platform, StringView title, uint32_t width, uint32_t height,
    CallbackInputMousePos callback_input_mouse_pos) {

    clock_gettime(CLOCK_MONOTONIC, &platform.time_start);

    log_debug("Creating window: '%s' %u x %u", title, width, height);

    platform.width = width;
    platform.height = height;
    platform.callback_input_mouse_pos = callback_input_mouse_pos;

    TRY(x_create(platform.xcb, static_cast<uint16_t>(width), static_cast<uint16_t>(height), "OpenGL Platform"));
    TRY(egl_create(platform.egl, platform.xcb));
    TRY(gfx_init(platform));

    platform.valid = true;

    return Error::Ok;
}

void destroy(Platform& platform) {
    log_debug("Exiting...");

    platform.valid = false;
    egl_destroy(platform.egl);
    x_destroy(platform.xcb);
}

Error window_map(Platform& platform) {
    inl_assert(platform.valid, "Invalid platform");

    xcb_map_window(platform.xcb.connection, platform.xcb.platform);
    xcb_flush(platform.xcb.connection);

    return Error::Ok;
}

void window_process_events(Platform& platform) {
    inl_assert(platform.valid, "Invalid platform");

    xcb_generic_event_t* xcb_event {};
    inl_defer(free(xcb_event));

    while ((xcb_event = xcb_poll_for_event(platform.xcb.connection))) {
        switch (xcb_event->response_type & XCB_RESPOSE_TYPE_MASK) {
        case XCB_EXPOSE: {
            break;
        }
        case XCB_CONFIGURE_NOTIFY: {
            // TODO: Fix window vs graphic resize black box's
            xcb_configure_notify_event_t* configure_event = (xcb_configure_notify_event_t*)xcb_event;
            platform.width = configure_event->width;
            platform.height = configure_event->height;
            gfx_resize(platform, configure_event->width, configure_event->height);
            break;
        }
        case XCB_KEY_PRESS:
        case XCB_KEY_RELEASE: {
            xcb_key_press_event_t* key_event = (xcb_key_press_event_t*)xcb_event;
            window_handle_key_input(platform, *key_event);
            break;
        }
        case XCB_BUTTON_PRESS: {
            break;
        }
        case XCB_BUTTON_RELEASE: {
            break;
        }
        case XCB_GE_GENERIC: {
            xcb_ge_generic_event_t* generic_event = (xcb_ge_generic_event_t*)xcb_event;

            if (generic_event->event_type == XCB_INPUT_RAW_MOTION) {
                xcb_input_raw_motion_event_t* motion_event = (xcb_input_raw_motion_event_t*)xcb_event;
                window_handle_mouse_input(platform, *motion_event);
            }
            break;
        }

        case XCB_MOTION_NOTIFY: {
            break;
        }
        case XCB_ENTER_NOTIFY: {
            if (x_grab_cursor(platform.xcb) != Error::Ok) platform.should_exit = true;
            break;
        }
        case XCB_LEAVE_NOTIFY: {
            if (x_ungrab_cursor(platform.xcb) != Error::Ok) platform.should_exit = true;
            break;
        }
        case XCB_FOCUS_IN: {
            break;
        }
        case XCB_FOCUS_OUT: {
            break;
        }
        case XCB_CLIENT_MESSAGE: {
            xcb_client_message_event_t* client_msg_event = (xcb_client_message_event_t*)xcb_event;
            if (client_msg_event->type == platform.xcb.atom_wm_protocols
                && client_msg_event->data.data32[0] == platform.xcb.atom_wm_delete) {
                platform.should_exit = true;
                break;
            }
            break;
        }
        default: {
            break;
        }
        }
    }
}

void window_resize(Platform& platform, uint32_t width, uint32_t height) {
    inl_assert(platform.valid, "Invalid platform");

    const static uint32_t values[] = { width, height };
    platform.width = width;
    platform.height = height;

    xcb_configure_window(
        platform.xcb.connection, platform.xcb.platform, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
    xcb_flush(platform.xcb.connection);
}

bool window_should_exit(Platform& platform) { return platform.should_exit; }

float elapsed_time(Platform& platform) {
    timespec now {};
    clock_gettime(CLOCK_MONOTONIC, &now);

    timespec td {
        now.tv_sec - platform.time_start.tv_sec,
        now.tv_nsec - platform.time_start.tv_nsec,
    };

    static constexpr uint32_t NS_PER_SEC = 1000000000;
    float elapsed = float(td.tv_sec) + (float(td.tv_nsec) / NS_PER_SEC);

    return elapsed;
}

void* mem_alloc(size_t size) {
    void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    inl_assert(mem != MAP_FAILED, "mmap failed");
    return mem;
}

void mem_free(void* mem, size_t size) {
    int result = munmap(mem, size);
    inl_assert(result == 0, "unmap failed");
}

} // namespace inl::platform
