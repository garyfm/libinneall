#include <libinneall/base/string_view.hpp>
#include <libinneall/math/math.hpp>

#include <stdint.h>

namespace inl {

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
