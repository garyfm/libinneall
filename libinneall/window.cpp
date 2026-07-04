#include <libinneall/base/log.hpp>
#include <libinneall/window.hpp>

namespace {

void APIENTRY opengl_debug_callback(GLenum source, GLenum type, uint32_t id, GLenum severity,
    [[maybe_unused]] GLsizei length, const char* message, [[maybe_unused]] const void* userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

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

} // namespace

namespace inl {

Error Window::create(Window& window, uint32_t width, uint32_t height, StringView title, InputCallback input_callback,
    [[maybe_unused]] MouseCallback mouse_callback, [[maybe_unused]] ScrollCallback scroll_callback,
    [[maybe_unused]] ResizeCallback resize_callback) {

    window.m_width = width;
    window.m_height = height;
    window.m_title = title;
    window.m_input_callback = input_callback;

    log_debug("Creating window: '%s' %u x %u", window.m_title.data(), window.m_width, window.m_height);

    TRY(platform::window_create(window.m_native_window));

    TRY(platform::window_map(window.m_native_window));

    if (!gladLoadGLLoader((GLADloadproc)eglGetProcAddress)) {
        return Error::WindowGladFailedToLoad;
    }

    // glfwSetFramebufferSizeCallback(window.m_native_window.get(), resize_callback);
    glViewport(0, 0, window.m_width, window.m_height);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_debug_callback, nullptr);

    // NOTE: This can be used to filter opengl debug messages
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // glfwSetInputMode(window.m_native_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glfwSetCursorPosCallback(window.m_native_window.get(), mouse_callback);
    // glfwSetScrollCallback(window.m_native_window.get(), scroll_callback);

    return Error::Ok;
}

Window::~Window() { platform::window_destroy(m_native_window); }

void Window::process_input() {
    // glfwPollEvents();
    // if (glfwGetKey(m_native_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    //     glfwSetWindowShouldClose(m_native_window.get(), true);
    // }

    // m_input_callback(m_native_window.get());
}

void Window::swap_buffers() { platform::swap_buffers(m_native_window); }

void Window::resize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}
} // namespace inl
