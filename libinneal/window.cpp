#include <libinneal/log.hpp>
#include <libinneal/window.hpp>
#include <memory>

namespace inl {

namespace {
    void error_callback(int error, const char* description) { log::error("GLFW error: {}({})", description, error); }
    void framebuffer_resize_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}

Window::Window(unsigned width, unsigned height, const std::string& title)
    : m_width { width }
    , m_height { height }
    , m_title { title } {

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

    glfwSetFramebufferSizeCallback(m_window.get(), framebuffer_resize_callback);
    glViewport(0, 0, m_width, m_height);
}

Window::~Window() { glfwTerminate(); }

void Window::process_input() {

    glfwPollEvents();
    if (glfwGetKey(m_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window.get(), true);
    }
}

void Window::swap_buffers() { glfwSwapBuffers(m_window.get()); }

} // namespace inl
