#include <libinneall/base/string_view.hpp>
#include <libinneall/math/math.hpp>

#include <stdint.h>

namespace inl {

std::optional<size_t> StringView::find(StringView needle, size_t start_pos) {
    if (m_size < needle.size() || start_pos > m_size) {
        return std::nullopt;
    }

    for (size_t i = start_pos; i < m_size; ++i) {

        if (m_data[i] != needle[0]) {
            continue;
        }

        if (m_size - i < needle.size()) {
            return std::nullopt;
        }

        if (memcmp(m_data + i, needle.data(), needle.size()) == 0) {
            return i;
        }
    }

    return std::nullopt;
}

std::optional<size_t> StringView::rfind(StringView needle, size_t start_pos) {
    if (m_size < needle.size() || start_pos > m_size) {
        return std::nullopt;
    }

    size_t pos = (start_pos != 0) ? start_pos : m_size;

    for (int32_t i = static_cast<int32_t>(pos); i >= 0; --i) {

        if (m_data[i] != needle[0]) {
            continue;
        }

        if (m_size - i < needle.size()) {
            return std::nullopt;
        }

        if (memcmp(m_data + i, needle.data(), needle.size()) == 0) {
            return i;
        }
    }

    return std::nullopt;
}

StringView trim_left(StringView sv) {
    size_t cursor { 0 };
    for (; cursor < sv.size(); ++cursor) {
        if (!inl::isspace(sv[cursor])) {
            break;
        }
    }

    return sv.substr(cursor, sv.size() - cursor);
}

StringView trim_right(StringView sv) {
    int32_t cursor { static_cast<int32_t>(sv.size() - 1) };
    for (; cursor >= 0; --cursor) {
        if (!inl::isspace(sv[cursor])) {
            break;
        }
    }

    return sv.substr(0, cursor + 1);
}

StringView trim(StringView sv) {
    StringView left { trim_left(sv) };
    return trim_right(left);
}

Cut cut(StringView sv, char deliminator) {

    size_t cursor { 0 };
    Cut result {};
    for (; cursor < sv.size(); ++cursor) {
        if (sv[cursor] == deliminator) {
            result.left = sv.substr(0, cursor);
            result.right = sv.substr(cursor + 1, sv.size() - (cursor + 1));
            result.success = true;
            break;
        }
    }

    if (cursor == sv.size()) {
        result.left = sv;
        result.right = "";
        result.success = false;
    }

    return result;
}

} // namepace inl
