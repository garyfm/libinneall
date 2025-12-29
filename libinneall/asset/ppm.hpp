#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>

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
    std::size_t width;
    std::size_t height;
    std::uint16_t max_value;
    // TODO: should this own the data ?
    std::vector<std::uint8_t> pixel_data;
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
Result<Image> load(std::span<std::uint8_t> raw_data);

} // namespace inl
