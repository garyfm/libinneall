#pragma once

#include <libinneall/base/assert.hpp>

#include <type_traits>

namespace inl {

// Tag used to denote an Option with no value
struct NoneType {
    explicit NoneType() = default;
};

static constexpr NoneType None {};

template <typename T>
concept PodType = std::is_trivially_copyable_v<T>;

template <PodType T> class Option {
public:
    Option()
        : m_has_value { false }
        , m_value {} { };

    Option(NoneType)
        : m_has_value { false }
        , m_value {} { };

    Option(T value)
        : m_has_value { true }
        , m_value { value } { };

    Option<T>& operator=(T value) {
        m_has_value = true;
        m_value = value;
        return *this;
    }

    void reset() { m_has_value = false; }

    bool has_value() const { return m_has_value; }
    operator bool() const { return m_has_value; }

    T* operator->() {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return &m_value;
    }

    T const* operator->() const {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return &m_value;
    }

    T const& operator*() const {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return m_value;
    }

    T value() {
        INL_ASSERT(m_has_value, "Option does not contain a value");
        return m_value;
    }

    T value_or(T const& alternative) const {
        if (m_has_value) {
            return m_value;
        }
        return alternative;
    }

    template <typename F> Option<std::invoke_result_t<F, T>> map(F&& fn) {
        if (m_has_value) {
            return fn(m_value);
        }
        return None;
    }

    template <typename F> std::invoke_result_t<F, T> and_then(F&& fn) {
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
    T m_value {};
};

} // namespace inl
