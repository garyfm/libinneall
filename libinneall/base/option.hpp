#pragma once

#include <libinneall/base/assert.hpp>

#include <type_traits>

// TODO: Is the complexity of this class really worth it ?
// I'm not convinced that this is better than a C style error + out_param.
namespace inl {

// Tag used to denote an Option with no value
struct NoneType {
    explicit NoneType() = default;
};

static constexpr NoneType None {};

template <typename T> class Option {
public:
    Option()
        : m_has_value { false }
        , m_none {} { };

    Option(NoneType)
        : m_has_value { false }
        , m_none {} { };

    // Copy
    Option(Option<T> const& other)
        requires std::is_copy_constructible_v<T>
        : m_has_value { other.m_has_value }
        , m_none {} {

        if (other.m_has_value) {
            new (&m_value) T(other.m_value);
        }
    }

    Option(Option<T> const& other)
        requires(!std::is_copy_constructible_v<T>)
    = delete;

    Option(T const& value)
        requires std::is_copy_constructible_v<T>
        : m_has_value { true } {
        new (&m_value) T(value);
    }

    Option(T const& value)
        requires(!std::is_copy_constructible_v<T>)
    = delete;

    Option<T>& operator=(Option<T> const& other)
        requires std::is_copy_constructible_v<T>
    {
        if (this != &other) {

            if (m_has_value && other.m_has_value) {
                m_value = other.m_value;
            } else if (!m_has_value && other.m_has_value) {
                new (&m_value) T(other.m_value);
                m_has_value = true;
            } else if (m_has_value && !other.m_has_value) {
                reset();
            }
        }

        return *this;
    }

    Option<T>& operator=(Option<T> const& other)
        requires(!std::is_copy_constructible_v<T>)
    = delete;

    Option<T>& operator=(T const& other)
        requires std::is_copy_constructible_v<T>
    {
        if (m_has_value) {
            m_value = other;

        } else {
            m_has_value = true;
            new (&m_value) T(other);
        }

        return *this;
    }

    // Move
    Option(Option<T>&& other)
        requires std::is_move_constructible_v<T>
        : m_has_value { other.m_has_value }
        , m_none {} {

        if (other.m_has_value) {
            new (&m_value) T(std::move(other.m_value));
        }
        other.reset();
    }

    Option(Option<T>&& other)
        requires(!std::is_move_constructible_v<T>)
    = delete;

    Option(T&& value)
        requires std::is_move_constructible_v<T>
        : m_has_value { true } {
        new (&m_value) T(std::move(value));
    }

    Option(T&& value)
        requires(!std::is_move_constructible_v<T>)
    = delete;

    Option<T>& operator=(Option<T>&& other)
        requires std::is_move_constructible_v<T>
    {

        if (this != &other) {

            if (m_has_value && other.m_has_value) {
                m_value = std::move(other.m_value);
            } else if (!m_has_value && other.m_has_value) {
                new (&m_value) T(std::move(other.m_value));
                m_has_value = true;
            } else if (m_has_value && !other.m_has_value) {
                reset();
            }

            other.reset();
        }

        return *this;
    }

    Option<T>& operator=(T&& other)
        requires std::is_move_constructible_v<T>
    {

        if (m_has_value) {
            m_value = std::move(other);
        } else {

            m_has_value = true;
            new (&m_value) T(std::move(other));
        }

        return *this;
    }

    Option<T>& operator=(Option<T>&& other)
        requires(!std::is_move_constructible_v<T>)
    = delete;

    ~Option() { reset(); };

    void reset() {
        if (m_has_value) {
            if constexpr (std::is_trivially_destructible_v<T> == false) {
                m_value.T::~T();
            }
            m_has_value = false;
        }
    }

    bool has_value() const { return m_has_value; }
    operator bool() const { return m_has_value; }

    T& operator*() {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return m_value;
    }

    T* operator->() {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return &m_value;
    }

    T const& operator*() const {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return m_value;
    }

    T const* operator->() const {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return &m_value;
    }

    T& value() {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return m_value;
    }

    T const& value() const {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return m_value;
    }

    T value_or(T const& alternative) const {
        if (m_has_value) {
            return m_value;
        }
        return alternative;
    }

    template <typename F> Option<std::invoke_result_t<F, T&>> map(F&& fn) & {
        if (m_has_value) {
            return fn(m_value);
        }
        return None;
    }

    template <typename F> Option<std::invoke_result_t<F, T const&>> map(F&& fn) const& {
        if (m_has_value) {
            return fn(m_value);
        }

        return None;
    }

    template <typename F> std::invoke_result_t<F, T&> and_then(F&& fn) & {
        if (m_has_value) {
            return fn(m_value);
        }

        return None;
    }

    template <typename F> std::invoke_result_t<F, T const&> and_then(F&& fn) const& {
        if (m_has_value) {
            return fn(m_value);
        }

        return None;
    }

    template <typename F> Option<T> or_else(F&& fn) {
        if (!m_has_value) {
            return fn();
        }

        return *this;
    }

private:
    bool m_has_value { false };

    // Union used to allocate memory for T but not consturct it.
    // Default initialisation will init m_none rather than T.
    // m_none serves no other purpose.
    union {
        T m_value;
        NoneType m_none {};
    };
};

} // namespace inl
