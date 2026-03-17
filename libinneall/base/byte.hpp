#pragma once

#include <libinneall/base/span.hpp>

#include <stdint.h>

namespace inl {

template <typename T> Span<uint8_t const> to_bytes(T const& convert) {
    return Span<uint8_t const> { reinterpret_cast<uint8_t const*>(&convert), sizeof(T) };
}

// TODO : This should take a span
template <typename T> Span<uint8_t const> to_bytes(T const& convert, size_t count) {
    return Span<uint8_t const> { reinterpret_cast<uint8_t const*>(&convert), sizeof(T) * count };
}
} // namespace
