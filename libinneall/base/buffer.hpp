#pragma once

#include <libinneall/base/arena.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/base/string_utils.hpp>
#include <libinneall/base/utility.hpp>

namespace inl {
template <PodType T> class Buffer {
public:
    Buffer(T* data, size_t capacity)
        : m_data { data }
        , m_size {}
        , m_capacity { capacity } { };

    void push(T const& ele) {
        inl_assert(m_size < m_capacity, "Exceeded buffer capacity");

        m_data[m_size] = ele;
        ++m_size;
    }

    T& operator[](size_t index) {
        inl_assert(index < m_size, "Out of bounds access");
        return m_data[index];
    }

    T const& operator[](size_t index) const {
        inl_assert(index < m_size, "Out of bounds access");
        return m_data[index];
    }

    operator Span<T>() { return { m_data, m_size }; }

    T* data() { return m_data; };
    T const* data() const { return m_data; };

    T* begin() { return m_data; };
    T const* begin() const { return m_data; };

    T* end() { return m_data + m_size; };
    T const* end() const { return m_data + m_size; };

    size_t size() const { return m_size; };
    size_t capacity() const { return m_capacity; };

private:
    T* m_data {};
    size_t m_size {};
    const size_t m_capacity {};
};

}; // namespace inl
