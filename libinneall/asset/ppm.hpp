#pragma once

#include <libinneall/base/span.hpp>

#include <expected>
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

    size_t size_bytes() const { return width * height * 3; }
};

enum class Error {
    InvalidFormat,
    UnsupportedFormat,
    EOFReachedUnexpectedly,
    FailedToExtractInteger,
    InvalidWidth,
    InvalidMaxValue,
};

template <typename T> using Result = std::expected<T, Error>;
Result<Image> load(ByteSpan raw_data);

Image flip_vertically(ByteSpan buffer, Image const& image);

} // namespace inl
