#pragma once

#include <utility>

namespace inl {

template <typename T, auto Deleter, T Sentinel = {}> class UniqueHandle {
public:
    explicit UniqueHandle(T resource) noexcept
        : m_resource { resource } { }

    ~UniqueHandle() {
        if ((m_resource != Sentinel)) {
            Deleter(m_resource);
        }
    }

    UniqueHandle(const UniqueHandle&) = delete;
    UniqueHandle operator=(const UniqueHandle&) = delete;

    UniqueHandle(UniqueHandle&& other) noexcept
        : m_resource { other.m_resource } {

        other.m_resource = Sentinel;
    }

    UniqueHandle& operator=(UniqueHandle&& other) noexcept {
        if (this != std::addressof(other)) {
            reset(); // Clear this instance
            m_resource = other.m_resource;
            other.m_resource = Sentinel;
        }
        return *this;
    }

    void reset(T new_resource = Sentinel) noexcept {
        if (m_resource != Sentinel) {
            Deleter(m_resource);
        }
        m_resource = new_resource;
    }

    T get() const noexcept { return m_resource; }

    operator T() const noexcept { return m_resource; }

    explicit operator bool() const noexcept { return m_resource != Sentinel; }

    T* operator&() noexcept { return std::addressof(m_resource); }

private:
    T m_resource {};
};

// template <typename T, typename Deleter> UniqueHandle(T, Deleter, T) -> UniqueHandle<T, Deleter, T {}>;
} // namespace inl
