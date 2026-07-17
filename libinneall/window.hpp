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

    static Error create(Window& window, uint32_t width, uint32_t height, StringView title,
        platform::CallbackInputKey callback_input_key, platform::CallbackInputMousePos callback_input_mouse_pos,
        [[maybe_unused]] ScrollCallback scroll_callback);

    void process_events();
    void swap_buffers();
    void resize(uint32_t width, uint32_t height);

    platform::Window& native_window() { return m_native_window; };
    uint32_t width() const { return m_native_window.width; }
    uint32_t height() const { return m_native_window.height; }
    float aspect_ratio() const { return static_cast<float>(width()) / static_cast<float>(height()); }

private:
    static constexpr size_t MAX_TITLE_SIZE { 128 };
    bool m_created { false };
    String<MAX_TITLE_SIZE> m_title {};

    platform::Window m_native_window;
};

} // namespace inl
