#pragma once

namespace inl {

template <typename T, auto Deleter> class UniquePtr {
public:
    explicit UniquePtr(T* resource) noexcept
        : m_resource { resource } { }

    ~UniquePtr() noexcept {
        if ((m_resource != nullptr)) {
            Deleter(m_resource);
        }
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept
        : m_resource { other.m_resource } {

        other.m_resource = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset(); // Clear this instance
            m_resource = other.m_resource;
            other.m_resource = nullptr;
        }
        return *this;
    }

    void reset(T* new_resource = nullptr) noexcept {
        if (m_resource != nullptr) {
            Deleter(m_resource);
        }
        m_resource = new_resource;
    }

    T* release() noexcept {
        T* tmp = m_resource;
        m_resource = nullptr;
        return tmp;
    }

    T& operator*() const { return *m_resource; }
    T* operator->() const { return m_resource; }
    T* get() const noexcept { return m_resource; }

    explicit operator bool() const noexcept { return m_resource != nullptr; }

private:
    T* m_resource {};
};

} // namespace inl
