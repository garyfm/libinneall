#include <libinneall/base/string.hpp>
#include <libinneall/math/math.hpp>

#include <stdint.h>

namespace inl {

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
