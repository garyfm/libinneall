#include <libinneall/base/string.hpp>
#include <libinneall/math/math.hpp>

#include <stdint.h>

namespace inl {

int memcmp(void const* lhs, void const* rhs, size_t size) {
    if (size == 0) {
        return 0;
    }
    INL_ASSERT(lhs != nullptr, "Invalid pointer");
    INL_ASSERT(rhs != nullptr, "Invalid pointer");

    auto lhs_bytes = reinterpret_cast<uint8_t const*>(lhs);
    auto rhs_bytes = reinterpret_cast<uint8_t const*>(rhs);

    for (size_t i = 0; i < size; ++i) {
        if (lhs_bytes[i] != rhs_bytes[i]) {
            return (lhs_bytes[i] < rhs_bytes[i]) ? -1 : 1;
        }
    }

    return 0;
}

void memcpy(void* dst, void const* src, size_t size) {
    if (size == 0) {
        return;
    }
    INL_ASSERT(dst != nullptr, "Invalid pointer");
    INL_ASSERT(src != nullptr, "Invalid pointer");

    auto dst_bytes = reinterpret_cast<uint8_t*>(dst);
    auto src_bytes = reinterpret_cast<uint8_t const*>(src);

    INL_ASSERT(dst_bytes + size <= src || src_bytes + size <= dst, "dst and src overlap");

    for (size_t i = 0; i < size; ++i) {
        dst_bytes[i] = src_bytes[i];
    }
}

std::string_view trim_left(std::string_view sv) {
    size_t cursor { 0 };
    for (; cursor < sv.size(); ++cursor) {
        if (!inl::isspace(sv[cursor])) {
            break;
        }
    }

    return sv.substr(cursor, sv.size());
}

std::string_view trim_right(std::string_view sv) {
    int32_t cursor { static_cast<int32_t>(sv.size() - 1) };
    for (; cursor >= 0; --cursor) {
        if (!inl::isspace(sv[cursor])) {
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

    size_t cursor { 0 };
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

uint8_t digit_count(uint32_t number) {

    // clang-format off
    if (number < 10) return 1;
    if (number < 100) return 2;
    if (number < 1'000) return 3;
    if (number < 10'000) return 4;
    if (number < 100'000) return 5;
    if (number < 1'000'000) return 6;
    if (number < 10'000'000) return 7;
    if (number < 100'000'000) return 8;
    if (number < 1'000'000'000) return 9;
    // clang-format on

    return 10;
}

uint8_t digit_count(int32_t number) {

    // abs on INT32_MIN is UB
    if (number == INT32_MIN) {
        return 10;
    }

    int32_t num_abs = abs(number);
    // clang-format off
    if (num_abs < 10) return 1;
    if (num_abs < 100) return 2;
    if (num_abs < 1'000) return 3;
    if (num_abs < 10'000) return 4;
    if (num_abs < 100'000) return 5;
    if (num_abs < 1'000'000) return 6;
    if (num_abs < 10'000'000) return 7;
    if (num_abs < 100'000'000) return 8;
    if (num_abs < 1'000'000'000) return 9;
    // clang-format on

    return 10;
}

static void to_string(String<MAX_STRING_SIZE_OF_NUMBER>& buffer, uint8_t n_digits, uint32_t number) {
    do {
        uint8_t units = static_cast<uint8_t>(number % 10);
        buffer[n_digits - 1] = '0' + static_cast<char>(units);
        number = number / 10;
        --n_digits;
    } while (number != 0);
}

String<MAX_STRING_SIZE_OF_NUMBER> to_string(int32_t number) {
    String<MAX_STRING_SIZE_OF_NUMBER> buffer;

    uint8_t n_digits = digit_count(number);

    if (number < 0) {
        ++n_digits;
    }
    buffer.resize(n_digits);

    if (number < 0) {
        buffer[0] = '-';
    }

    to_string(buffer, n_digits, abs(number));

    return buffer;
}

String<MAX_STRING_SIZE_OF_NUMBER> to_string(uint32_t number) {
    String<MAX_STRING_SIZE_OF_NUMBER> buffer;

    uint8_t n_digits = digit_count(number);

    buffer.resize(n_digits);

    to_string(buffer, n_digits, number);

    return buffer;
}

} // namepace inl
