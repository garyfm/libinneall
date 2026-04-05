#pragma once

#include <libinneall/base/assert.hpp>
#include <libinneall/base/hash.hpp>

#include <stdint.h>
#include <string_view>

namespace inl {

int memcmp(void const* lhs, void const* rhs, size_t size);
void memcpy(void* dst, void const* src, size_t size);

inline constexpr size_t strlen(char const* str) {
    INL_ASSERT(str != nullptr, "Invalid pointer");

    char const* end = str;

    while (*end != '\0') {
        ++end;
    }

    return end - str;
}

template <size_t N> inline constexpr size_t strlen(char const (&str)[N]) {
    size_t len { 0 };
    while ((len < N) && (str[len] != '\0')) {
        ++len;
    }

    return len;
}

inline constexpr bool isspace(char c) {
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

inline constexpr bool isdigit(char c) { return (c >= '0' && c <= '9'); }

template <size_t N> class String {
public:
    static_assert(N > 0, "String size must be greater than zero");

    String() = default;

    template <size_t LenNullTerm> String(char const (&c_str)[LenNullTerm]) {
        static_assert(LenNullTerm - 1 <= N, "c_str is greater than capactiry");

        memcpy(m_buffer, c_str, LenNullTerm - 1);

        m_size = LenNullTerm - 1;
        m_buffer[m_size] = '\0';
    }

    template <size_t Len> explicit String(String<Len> const& other) {
        static_assert(Len <= N, "Other String is greater than capactiry");

        memcpy(m_buffer, other.data(), other.size());
        m_size = other.size();
        m_buffer[m_size] = '\0';
    }

    String(std::string_view sv) {
        INL_ASSERT(sv.size() <= N, "String view is to greater than capacity");

        if (sv.size() > 0) {
            memcpy(m_buffer, sv.data(), sv.size());
        }

        m_size = sv.size();
        m_buffer[m_size] = '\0';
    }

    String(size_t size) {
        INL_ASSERT(size <= N, "Size is greater than capacity");

        m_size = size;
    }

    template <size_t Len> String& operator=(String<Len> const& other) {
        static_assert(Len <= N, "Other String is greater than capactiry");

        if (reinterpret_cast<void const*>(this) == reinterpret_cast<void const*>(&other)) {
            return *this;
        }

        memcpy(m_buffer, other.data(), other.size());
        m_size = other.size();
        m_buffer[m_size] = '\0';

        return *this;
    }

    template <size_t Len> bool operator==(const String<Len>& other) const {
        if (m_size != other.size()) {
            return false;
        }

        return memcmp(m_buffer, other.data(), m_size) == 0;
    }

    bool operator==(std::string_view other) const {

        if (m_size != other.size()) {
            return false;
        }

        return memcmp(m_buffer, other.data(), m_size) == 0;
    }

    template <size_t LenNullTerm> bool operator==(const char (&str)[LenNullTerm]) const {
        if (m_size != LenNullTerm - 1) {
            return false;
        }
        return memcmp(m_buffer, str, m_size) == 0;
    }

    char& operator[](size_t index) {
        INL_ASSERT(index < m_size, "Out of bounds access");
        return m_buffer[index];
    }

    char const& operator[](size_t index) const {
        INL_ASSERT(index < m_size, "Out of bounds access");
        return m_buffer[index];
    }

    operator std::string_view() const { return { data(), size() }; }

    void resize(size_t new_size) {
        INL_ASSERT(new_size <= m_capacity, "new_size is greater than capactiy");
        m_size = new_size;
        m_buffer[m_size] = '\0';
    }

    String& append(std::string_view sv) {
        INL_ASSERT(m_size + sv.size() <= m_capacity, "append string is greater than capactiy");

        if (sv.size() > 0) {
            memcpy(m_buffer + m_size, sv.data(), sv.size());
            m_size += sv.size();
            m_buffer[m_size] = '\0';
        }

        return *this;
    }

    String& append(char c) {

        append({ &c, 1 });

        return *this;
    }

    void clear() {
        m_size = 0;
        m_buffer[m_size] = '\0';
    }

    String& replace(std::string_view sv, size_t pos = 0) {
        INL_ASSERT(sv.size() + pos <= m_size, "replace string overflows the capactiy");

        if (sv.size() > 0) {
            memcpy(m_buffer + pos, sv.data(), sv.size());
        }

        return *this;
    }

    String& overwrite(std::string_view sv, size_t pos = 0) {
        INL_ASSERT(sv.size() + pos <= m_capacity, "overwrite string overflows the capactiy");

        if (sv.size() > 0) {
            memcpy(m_buffer + pos, sv.data(), sv.size());
            m_size = pos + sv.size();
            m_buffer[m_size] = '\0';
        }

        return *this;
    }

    bool empty() { return m_size == 0; }

    char* data() { return m_buffer; }
    char const* data() const { return m_buffer; }

    char* begin() { return m_buffer; };
    char const* begin() const { return m_buffer; }

    char* end() { return m_buffer + m_size; };
    char const* end() const { return m_buffer + m_size; }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }

private:
    char m_buffer[N + 1] {};
    size_t m_size {};
    static constexpr size_t m_capacity { N };
};

// String literals will include the null terminator in the size.
// This is already accounted for in String so we decrease
// the size of the literal by 1
template <size_t Len> String(const char (&str)[Len]) -> String<Len - 1>;

struct StringHash {
    template <size_t N> size_t operator()(inl::String<N> const& str) const { return hash_fnv1a(str); }

    size_t operator()(std::string_view sv) const { return hash_fnv1a(sv); }
};

struct Cut {
    std::string_view left;
    std::string_view right;
    bool success;
};

std::string_view trim_left(std::string_view sv);
std::string_view trim_right(std::string_view sv);
std::string_view trim(std::string_view sv);

Cut cut(std::string_view sv, char deliminator);

uint8_t digit_count(uint32_t number);

constexpr size_t MAX_STRING_SIZE_OF_NUMBER = 11;

String<MAX_STRING_SIZE_OF_NUMBER> to_string(int32_t number);
String<MAX_STRING_SIZE_OF_NUMBER> to_string(uint32_t number);

} // namespace inl
