#pragma once

#include <libinneall/base/string.hpp>

#define GLFW_INCLUDE_NONE
#include <subprojects/glad/include/glad/glad.h>
#include <subprojects/glfw-3.4/include/GLFW/glfw3.h>

#include <functional>
#include <memory>

namespace inl {

class Window {
public:
    using InputCallback = std::function<void(GLFWwindow*)>;
    using MouseCallback = void (*)(GLFWwindow*, double, double);
    using ScrollCallback = void (*)(GLFWwindow*, double, double);
    using ResizeCallback = void (*)(GLFWwindow*, int32_t, int32_t);

    Window(uint32_t width, uint32_t height, StringView title, InputCallback input_callback,
        MouseCallback mouse_callback, ScrollCallback scroll_callback, ResizeCallback resize_callback);

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&&) = default;
    Window& operator=(Window&&) = default;

    ~Window();

    GLFWwindow* handle() const { return m_window.get(); }
    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    float aspect_ratio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }

    void process_input();
    void swap_buffers();
    void resize(uint32_t width, uint32_t height);

private:
    static constexpr size_t MAX_TITLE_SIZE { 128 };
    uint32_t m_width { 0 };
    uint32_t m_height { 0 };
    String<MAX_TITLE_SIZE> m_title {};

    static void framebuffer_resize_callback([[maybe_unused]] GLFWwindow* window, int32_t width, int32_t height);
    std::unique_ptr<GLFWwindow, void (*)(GLFWwindow*)> m_window { nullptr, glfwDestroyWindow };

    // TODO: Temporary solution to allow game to handle input
    InputCallback m_input_callback;
};

} // namespace inl
