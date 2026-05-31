#pragma once

#include <libinneall/base/arena.hpp>
#include <libinneall/base/error.hpp>
#include <libinneall/base/span.hpp>

#include <stddef.h>
#include <stdint.h>

namespace inl::ppm {

enum class Format {
    // Unsupported
    P1,
    P2,
    P3,
    P4,
    P5,
    // Supported
    P6,
};

struct Image {
    Format format;
    size_t width;
    size_t height;
    uint16_t max_value;
    ByteSpan pixel_data;

    static constexpr uint8_t n_channels = 3;

    size_t size_bytes() const { return width * height * n_channels; }
    size_t row_size_bytes() const { return width * n_channels; }
    size_t height_size_bytes() const { return height * n_channels; }
};

Error load(Image& image, ByteSpan raw_data);
void flip_vertically(Arena& arena, Image& image);

} // namespace inl
