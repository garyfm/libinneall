#pragma once

#define GLFW_INCLUDE_NONE
#include <subprojects/glad/include/glad/glad.h>
#include <subprojects/glfw-3.4/include/GLFW/glfw3.h>

#include <memory>
#include <string>

namespace inl {

class Window {
public:
    Window(unsigned width, unsigned height, const std::string& title);

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = default;
    Window& operator=(Window&&) = default;

    ~Window();

    GLFWwindow* native_handle() const { return m_window.get(); }
    unsigned width() const { return m_width; }
    unsigned heght() const { return m_height; }

    void process_input();
    void swap_buffers();

private:
    unsigned m_width { 0 };
    unsigned m_height { 0 };
    std::string m_title {};

    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> m_window { nullptr, glfwDestroyWindow };
};

} // namespace inl
