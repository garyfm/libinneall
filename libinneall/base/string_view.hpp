#pragma once

#include <libinneall/base/assert.hpp>
#include <libinneall/base/option.hpp>
#include <libinneall/base/string_utils.hpp>

#include <stddef.h>

namespace inl {

class StringView {
public:
    StringView() = default;

    StringView(char const* data, size_t size)
        : m_data { data }
        , m_size { size } { }

    constexpr StringView(char const* data)
        : m_data { data }
        , m_size { strlen(data) } { }

    template <size_t N>
    constexpr StringView(char const (&c_str)[N])
        : m_data { c_str }
        , m_size { N - 1 } { }

    bool operator==(StringView other) const {

        if (m_size != other.size()) {
            return false;
        }

        return memcmp(m_data, other.data(), m_size) == 0;
    }

    char const& operator[](size_t index) const {
        INL_ASSERT(index < m_size, "Out of bounds access");
        return m_data[index];
    }

    StringView substr(size_t offset, size_t size) const {
        INL_ASSERT(offset + size <= m_size, "Inavlid offset + size");
        return { m_data + offset, size };
    }

    Option<size_t> find(StringView needle, size_t start_pos = 0);
    Option<size_t> rfind(StringView needle, size_t start_pos = 0);

    char const* data() const { return m_data; };

    char const* begin() const { return m_data; };
    char const* end() const { return m_data + m_size; };

    constexpr size_t size() const { return m_size; };
    constexpr size_t size_bytes() const { return m_size * sizeof(char); };

    constexpr bool empty() { return m_size == 0; }

private:
    char const* m_data {};
    size_t m_size {};
};

struct Cut {
    StringView left;
    StringView right;
    bool success;
};

StringView trim_left(StringView sv);
StringView trim_right(StringView sv);
StringView trim(StringView sv);

Cut cut(StringView sv, char deliminator);

} // namespace inl
