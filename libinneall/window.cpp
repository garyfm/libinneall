#include <libinneall/base/log.hpp>
#include <libinneall/window.hpp>

#include <memory>

namespace inl {

namespace {
    void error_callback(int error, const char* description) { log::error("GLFW error: {}({})", description, error); }

    void APIENTRY opengl_debug_callback(GLenum source, GLenum type, unsigned int id, GLenum severity,
        [[maybe_unused]] GLsizei length, const char* message, [[maybe_unused]] const void* userParam) {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;

        std::string_view source_str {};
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

        std::string_view type_str {};
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

        std::string_view severity_str {};
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

        log::info("OpenGL debug callback - servierty: {} source: {} type: {} msg: {} ", severity_str, source_str,
            type_str, message);

        if (type == GL_DEBUG_TYPE_ERROR) {
            throw std::runtime_error("OpenGL debug error");
        }
    }
}

Window::Window(unsigned width, unsigned height, const std::string& title, InputCallback input_callback,
    MouseCallback mouse_callback, ScrollCallback scroll_callback, ResizeCallback resize_callback)
    : m_width { width }
    , m_height { height }
    , m_title { title }
    , m_input_callback { input_callback } {
    log::debug("Creating window: {} {}x{}", m_title, m_width, m_height);

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window.reset(glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr));

    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window.get());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to load GLAD");
    }

    glfwSetFramebufferSizeCallback(m_window.get(), resize_callback);
    glViewport(0, 0, m_width, m_height);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_debug_callback, nullptr);

    // NOTE: This can be used to filter opengl debug messages
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(m_window.get(), mouse_callback);
    glfwSetScrollCallback(m_window.get(), scroll_callback);
}

Window::~Window() { glfwTerminate(); }

void Window::process_input() {
    glfwPollEvents();
    if (glfwGetKey(m_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window.get(), true);
    }

    m_input_callback(m_window.get());
}

void Window::swap_buffers() { glfwSwapBuffers(m_window.get()); }

void Window::resize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;
    glViewport(0, 0, m_width, m_height);
}
} // namespace inl
