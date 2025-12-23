#include <libinneall/base/string.hpp>

namespace inl {

std::string_view trim_left(std::string_view sv) {
    std::size_t cursor { 0 };
    for (; cursor < sv.size(); ++cursor) {
        if (!std::isspace(sv[cursor])) {
            break;
        }
    }

    return sv.substr(cursor, sv.size());
}

std::string_view trim_right(std::string_view sv) {
    int cursor { static_cast<int>(sv.size() - 1) };
    for (; cursor >= 0; --cursor) {
        if (!std::isspace(sv[cursor])) {
            break;
        }
    }

    return sv.substr(0, cursor + 1);
}

std::string_view trim(std::string_view sv) {
    std::string_view left { trim_left(sv) };
    return trim_right(left);
}

Cut cut(std::string_view sv, char deliminator) {

    std::size_t cursor { 0 };
    Cut result {};
    for (; cursor < sv.size(); ++cursor) {
        if (sv[cursor] == deliminator) {
            result.left = sv.substr(0, cursor);
            result.right = sv.substr(cursor + 1, sv.size());
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
