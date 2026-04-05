#pragma once

#include <libinneall/base/assert.hpp>
#include <libinneall/base/span.hpp>

#include <stddef.h>

namespace inl {

template <typename T, size_t N> struct Array {

    static_assert(N > 0, "Array size must be greater than zero");

    bool operator==(const Array& other) const = default;

    T& operator[](size_t index) {
        INL_ASSERT(index < N, "Out of bounds access");
        return _m_elements[index];
    }

    T const& operator[](size_t index) const {
        INL_ASSERT(index < N, "Out of bounds access");
        return _m_elements[index];
    }

    operator Span<T>() { return { _m_elements, N }; }

    T* data() { return _m_elements; };
    T const* data() const { return _m_elements; };

    T* begin() { return _m_elements; };
    T const* begin() const { return _m_elements; };

    T* end() { return _m_elements + N; };
    T const* end() const { return _m_elements + N; };

    constexpr size_t size() const { return N; };

    // NOTE: Public to ensure the class remains an aggregate type
    // Avoid accessing directly
    T _m_elements[N] {};
};

} // namespace inl
