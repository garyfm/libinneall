#pragma once

#include <libinneall/base/error.hpp>
#include <libinneall/base/string.hpp>
#include <libinneall/base/unique_ptr.hpp>
#include <libinneall/base/utility.hpp>
#include <libinneall/platform/platform.hpp>

#include <subprojects/glad/include/glad/glad.h>

namespace inl {

class Window {
public:
    using InputCallback = void (*)(platform::Window&);
    using MouseCallback = void (*)(platform::Window&, double, double);
    using ScrollCallback = void (*)(platform::Window&, double, double);
    using ResizeCallback = void (*)(platform::Window&, int32_t, int32_t);

    Window() = default;
    ~Window();
    INL_DEL_COPY_MOVE(Window);

    static Error create(Window& window, uint32_t width, uint32_t height, StringView title, InputCallback input_callback,
        MouseCallback mouse_callback, ScrollCallback scroll_callback, ResizeCallback resize_callback);

    // GLFWwindow* handle() const { return m_window.get(); }
    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }
    float aspect_ratio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }

    void process_input();
    void swap_buffers();
    void resize(uint32_t width, uint32_t height);

private:
    static constexpr size_t MAX_TITLE_SIZE { 128 };
    bool m_created { false };
    uint32_t m_width { 0 };
    uint32_t m_height { 0 };
    String<MAX_TITLE_SIZE> m_title {};

    static void framebuffer_resize_callback([[maybe_unused]] platform::Window& window, int32_t width, int32_t height);
    // UniquePtr<GLFWwindow, glfwDestroyWindow> m_window { nullptr };
    platform::Window m_native_window;

    // TODO: Temporary solution to allow game to handle input
    InputCallback m_input_callback;
};

} // namespace inl
