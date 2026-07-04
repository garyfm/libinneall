#include <libinneall/base/log.hpp>
#include <libinneall/platform/platform.hpp>

#include <EGL/eglext.h>
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <unistd.h>
#include <xcb/xcb.h>

namespace inl::platform {

static Error xcb_connection_create(Window& window) {
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
    window.xcb_window = xcb_generate_id(window.xcb_conn);
    uint32_t window_values[] = {
        0,
        XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY,
        XCB_GRAVITY_STATIC,
    };

    xcb_void_cookie_t cookie = xcb_create_window_checked(window.xcb_conn, XCB_COPY_FROM_PARENT, window.xcb_window,
        screen->root, 0, 0, 800, 600, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
        XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_BIT_GRAVITY, window_values);

    xcb_generic_error_t* error = xcb_request_check(window.xcb_conn, cookie);

    if (error != NULL) {
        log_error("XCB error: %x", error->error_code);
        free(error);
        return Error::PlatformFailedToCreateWindow;
    }

    free(error);

    // Set the window name
    xcb_change_property(window.xcb_conn, XCB_PROP_MODE_REPLACE, window.xcb_window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
        sizeof("OpenGL Window") - 1, "OpenGL Window");

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

Error window_map(Window& window) {
    inl_assert(window.xcb_conn != nullptr, "Invalid window");

    xcb_map_window(window.xcb_conn, window.xcb_window);
    xcb_flush(window.xcb_conn);

    return Error::Ok;
}

Error window_create(Window& window) {

    TRY(xcb_connection_create(window));
    TRY(egl_ctx_create(window));

    return Error::Ok;
}

void window_destroy(Window& window) {
    eglMakeCurrent(window.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglDestroyContext(window.display, window.context);
    eglDestroySurface(window.display, window.surface);

    eglTerminate(window.display);
    xcb_destroy_window(window.xcb_conn, window.xcb_window);
    xcb_disconnect(window.xcb_conn);
}

void swap_buffers(Window& window) {
    inl_assert(window.xcb_conn != nullptr, "Invalid window");
    eglSwapBuffers(window.display, window.surface);
}

float get_time() { return 0.0; }

} // namespace inl::platform
