#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <vector>

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
    // TODO: should this own the data ?
    std::vector<uint8_t> pixel_data;
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
Result<Image> load(std::span<uint8_t> raw_data);

Image flip_vertically(Image const& image);

} // namespace inl
