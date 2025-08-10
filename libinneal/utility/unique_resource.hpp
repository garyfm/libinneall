#pragma once

#include <type_traits>

namespace inl {

template <typename T, typename Deleter, T Sentinel = {}>
class UniqueResource {
public:
    explicit UniqueResource(T resource, Deleter deleter) noexcept
        : m_resource { resource }
        , m_deleter { std::move(deleter) } {
    }

    ~UniqueResource() {
        if ((m_resource != Sentinel) && m_deleter) {
            m_deleter(m_resource);
        }
    }

    UniqueResource(const UniqueResource&) = delete;
    UniqueResource operator=(const UniqueResource&) = delete;

    UniqueResource(UniqueResource&& other) noexcept
        : m_resource { other.m_resource }
        , m_deleter { std::move(other.m_deleter) } {

        other.m_resource = Sentinel;
    }

    UniqueResource& operator=(UniqueResource&& other) noexcept {
        if (this != other) {
            reset(); // Clear this instance
            m_resource = other.m_resource;
            m_deleter = std::move(other.m_deleter);
            other.m_resource = Sentinel;
        }
        return *this;
    }

    void reset(UniqueResource new_resource = Sentinel) noexcept {
        if ((m_resource != Sentinel) && m_deleter) {
            m_deleter(m_resource);
        }
        m_resource = new_resource;
    }

    T get() const noexcept {
        return m_resource;
    }

    operator T() const noexcept {
        return m_resource;
    }

    explicit operator bool() const noexcept {
        return m_resource != Sentinel;
    }

private:
    T m_resource;
    Deleter m_deleter;
};

template <typename T, typename Deleter>
UniqueResource(T, Deleter, T) -> UniqueResource<T, Deleter, T {}>;
} // namespace inl
