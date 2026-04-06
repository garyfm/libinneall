#include <libinneall/base/string_utils.hpp>

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

} // namepace inl
