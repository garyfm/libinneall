#include <libinneall/base/log.hpp>
#include <libinneall/window.hpp>

namespace {

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

void callback_window_resize([[maybe_unused]] inl::platform::Platform& platform, int width, int height) {
    // NOTE: x11 will handle resizing the window if the user changes its size so only update the opengl view port
    glViewport(0, 0, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}

} // namespace

namespace inl {

Error Window::create(Window& window, uint32_t width, uint32_t height, StringView title,
    platform::CallbackInputMousePos callback_input_mouse_pos, [[maybe_unused]] ScrollCallback scroll_callback) {

    window.m_title = title;

    log_debug("Creating window: '%s' %u x %u", window.m_title.data(), width, height);

    platform::initialize(window.m_platform);
    TRY(platform::window_create(window.m_platform, width, height, callback_window_resize, callback_input_mouse_pos));

    TRY(platform::window_map(window.m_platform));

    if (!gladLoadGLLoader((GLADloadproc)eglGetProcAddress)) {
        return Error::WindowGladFailedToLoad;
    }

    glViewport(0, 0, width, height);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_debug_callback, nullptr);

    // NOTE: This can be used to filter opengl debug messages
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    printf("GL: %d %d %d %d\n", vp[0], vp[1], vp[2], vp[3]);

    return Error::Ok;
}

Window::~Window() { platform::window_destroy(m_platform); }

void Window::process_events() { platform::window_process_events(m_platform); }

void Window::swap_buffers() { platform::swap_buffers(m_platform); }

void Window::resize(uint32_t width, uint32_t height) {
    // Resize the platfrom window and opengl view port
    platform::window_resize(m_platform, width, height);
    glViewport(0, 0, width, height);
}
} // namespace inl
