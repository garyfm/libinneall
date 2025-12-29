#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/string.hpp>

#include <algorithm>
#include <cctype>
#include <charconv>

namespace {

inl::ppm::Result<void> skip_whitespace(std::span<std::uint8_t> buffer, std::size_t& cursor) {

    unsigned char c {};
    while (cursor < buffer.size()) {
        c = buffer[cursor];

        if (std::isspace(c)) {
            ++cursor;
            continue;
        }

        if (c != '#') {
            break;
        }

        // Skip till the end of the comment
        while (cursor < buffer.size() && buffer[cursor] != '\r' && buffer[cursor] != '\n') {
            ++cursor;
        }
        // Continue to skip any white space after the comment
    }

    return {};
}

inl::ppm::Result<int> extract_int(std::span<std::uint8_t> buffer, std::size_t& cursor) {

    std::string str;
    while (cursor < buffer.size() && std::isdigit(buffer[cursor])) {
        str.push_back(buffer[cursor]);
        ++cursor;
    }

    if (str.empty()) {
        return std::unexpected(inl::ppm::Error::FailedToExtractInteger);
    }

    int result;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

    if (ec != std::errc()) {
        return std::unexpected(inl::ppm::Error::FailedToExtractInteger);
    }
    return result;
}
}

namespace inl::ppm {

Result<Image> load(std::span<std::uint8_t> raw_data) {

    std::size_t cursor { 0 };
    std::string format;
    format.push_back(static_cast<uint8_t>(raw_data[cursor]));
    ++cursor;
    format.push_back(static_cast<uint8_t>(raw_data[cursor]));
    ++cursor;

    if (format[0] != 'P') {
        return std::unexpected { Error::InvalidFormat };
    }

    if (format[1] != '6') {
        return std::unexpected { Error::UnsupportedFormat };
    }

    TRY(skip_whitespace(raw_data, cursor));
    Result<int> width = TRY(extract_int(raw_data, cursor));

    TRY(skip_whitespace(raw_data, cursor));
    Result<int> height = TRY(extract_int(raw_data, cursor));

    TRY(skip_whitespace(raw_data, cursor));
    Result<int> max_value = TRY(extract_int(raw_data, cursor));

    // Skip the single whitespace prior to the image data
    ++cursor;

    const Image image {
        .format { Format::P6 },
        .width { static_cast<std::size_t>(width.value()) },
        .height { static_cast<std::size_t>(height.value()) },
        .max_value { static_cast<std::uint16_t>(max_value.value()) },
        .pixel_data { raw_data.begin() + cursor, raw_data.end() },
    };

    INL_ASSERT((image.pixel_data.size() == image.width * image.height * 3), "PPM Image size is invalid");
    return image;
}

Image flip_vertically(Image const& image) {
    Image flipped {
        .format { image.format },
        .width { image.width },
        .height { image.height },
        .max_value { image.max_value },
        .pixel_data {},
    };

    log::debug("pixel_data: {}, width: {}, height: {}", image.pixel_data.size(), image.width, image.height);

    const uint8_t n_channels { 3 };
    const std::size_t row_size_bytes { image.width * n_channels };

    flipped.pixel_data.resize(image.pixel_data.size());
    log::debug("flipped size: {}", flipped.pixel_data.size());

    for (std::size_t row = 0; row < image.height; ++row) {

        std::size_t row_start { row * row_size_bytes };
        std::size_t row_end { row_start + row_size_bytes };
        std::size_t flipped_cursor { (image.height - 1 - row) * row_size_bytes };

        // log::debug("row: {}", row);
        std::copy(image.pixel_data.begin() + row_start, image.pixel_data.begin() + row_end,
            flipped.pixel_data.begin() + flipped_cursor);
    }

    return flipped;
}
} // namespace inl
