#pragma once

#include <span>
#include <stdint.h>

namespace inl {

template <typename T> std::span<uint8_t const> to_bytes(T const& convert) {
    return std::span<uint8_t const> { reinterpret_cast<uint8_t const*>(&convert), sizeof(T) };
}

template <typename T> std::span<uint8_t const> to_bytes(T const& convert, size_t count) {
    return std::span<uint8_t const> { reinterpret_cast<uint8_t const*>(&convert), sizeof(T) * count };
}

} // namespace
