#pragma once

#include <libinneall/base/span.hpp>

#include <stdint.h>

namespace inl {

template <typename T> Span<uint8_t const> to_bytes(T const& convert) {
    return Span<uint8_t const> { reinterpret_cast<uint8_t const*>(&convert), sizeof(T) };
}

template <typename T> Span<uint8_t const> to_bytes(Span<T> convert) {
    return Span<uint8_t const> { reinterpret_cast<uint8_t const*>(convert.data()), convert.size_bytes() };
}
} // namespace
