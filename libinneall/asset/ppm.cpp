#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/error.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/string.hpp>
#include <libinneall/base/string_utils.hpp>

#include <errno.h>

namespace {

void skip_whitespace(inl::Span<uint8_t> buffer, size_t& cursor) {

    unsigned char c {};
    while (cursor < buffer.size()) {
        c = buffer[cursor];

        if (inl::isspace(static_cast<char>(c))) {
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
}

inl::Error extract_int(inl::Span<uint8_t> buffer, int32_t& value, size_t& cursor) {

    inl::String<inl::MAX_STRING_SIZE_OF_NUMBER> str {};
    while (cursor < buffer.size() && inl::isdigit(buffer[cursor])) {
        str.append(buffer[cursor]);
        ++cursor;
    }

    if (str.empty()) {
        return inl::Error::PpmFailedToExtractInteger;
    }

    char* end;
    errno = 0;
    // TODO: Implement from_int rather than strtol
    long int result = strtol(str.data(), &end, 10);

    if (end == str.data())
        return inl::Error::PpmFailedToExtractInteger;

    if (errno == ERANGE)
        return inl::Error::PpmFailedToExtractInteger;

    value = static_cast<int32_t>(result);

    return inl::Error::Ok;
}
} // namespace

namespace inl::ppm {

Error load(Image& image, ByteSpan raw_data) {

    size_t cursor { 0 };
    static constexpr uint8_t FORMAT_SIZE { 2 };
    String<FORMAT_SIZE> format;
    format.append(static_cast<char>(raw_data[cursor]));
    ++cursor;
    format.append(static_cast<char>(raw_data[cursor]));
    ++cursor;

    if (format[0] != 'P') {
        return Error::PpmInvalidFormat;
    }

    if (format[1] != '6') {
        return Error::PpmUnsupportedFormat;
    }

    skip_whitespace(raw_data, cursor);
    int32_t width {};
    TRY(extract_int(raw_data, width, cursor));

    skip_whitespace(raw_data, cursor);
    int32_t height {};
    TRY(extract_int(raw_data, height, cursor));

    skip_whitespace(raw_data, cursor);
    int32_t max_value {};
    TRY(extract_int(raw_data, max_value, cursor));

    // Skip the single whitespace prior to the image data
    ++cursor;

    size_t size_bytes = static_cast<size_t>(width * height * 3);
    image.format = Format::P6;
    image.width = static_cast<size_t>(width);
    image.height = static_cast<size_t>(height);
    image.max_value = static_cast<uint16_t>(max_value);
    image.pixel_data = { raw_data.begin() + cursor, size_bytes };

    inl_assert((image.pixel_data.size() == image.width * image.height * 3), "PPM Image size is invalid");

    return Error::Ok;
}

void flip_vertically(Arena& arena, Image& image) {

    ArenaTemp arena_temp { arena };
    uint8_t* temp_row = arena_temp.arena->alloc_array<uint8_t>(image.row_size_bytes());

    for (size_t row = 0; row < image.height; ++row) {
        size_t top_cursor { row * image.row_size_bytes() };
        size_t bottom_cursor { (image.height - 1 - row) * image.row_size_bytes() };

        memcpy(temp_row, &image.pixel_data[top_cursor], image.row_size_bytes());
        memcpy(&image.pixel_data[top_cursor], &image.pixel_data[bottom_cursor], image.row_size_bytes());
        memcpy(&image.pixel_data[bottom_cursor], temp_row, image.row_size_bytes());
    }
}
} // namespace inl::ppm
