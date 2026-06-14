#pragma once
#include <libinneall/base/assert.hpp>

#include <stddef.h>
#include <stdint.h>

namespace inl {

template <typename T> class Span {
public:
    Span() = default;

    Span(T* data, size_t size)
        : m_data { data }
        , m_size { size } { };

    Span(T* begin, T* end)
        : m_data { begin }
        , m_size { static_cast<size_t>(end - begin) } {

        inl_assert(end >= begin, "end less than begin");
    };

    T& operator[](size_t index) {
        inl_assert(index < m_size, "Out of bounds access");
        return m_data[index];
    }

    bool operator==(Span<T> other) const {

        if (m_size != other.size()) {
            return false;
        }

        return memcmp(m_data, other.data(), m_size * sizeof(T)) == 0;
    }

    T const& operator[](size_t index) const {
        inl_assert(index < m_size, "Out of bounds access");
        return m_data[index];
    }

    Span<T> subspan(size_t offset, size_t size) const {
        inl_assert(offset + size <= m_size, "Inavlid offset + size");
        return { m_data + offset, size };
    }

    T* data() { return m_data; };
    T const* data() const { return m_data; };

    T* begin() { return m_data; };
    T const* begin() const { return m_data; };

    T* end() { return m_data + m_size; };
    T const* end() const { return m_data + m_size; };

    constexpr size_t size() const { return m_size; };
    constexpr size_t size_bytes() const { return m_size * sizeof(T); };

private:
    T* m_data {};
    size_t m_size {};
};

using ByteSpan = Span<uint8_t>;

} // namespace inl
