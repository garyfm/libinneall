#include <libinneall/asset/asset.hpp>
#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/renderer/texture.hpp>

#include <chrono>
#include <fstream>
#include <span>

namespace inl {
// TODO: Implement these with proper error handling
std::string read_file(std::filesystem::path path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        inl::log::error("Failed to open file: {}", path.c_str());
    }

    std::size_t file_size = std::filesystem::file_size(path);
    std::string contents(file_size, '\0');

    file.read(contents.data(), file_size);

    return contents;
}

void read_file(std::filesystem::path path, std::vector<std::uint8_t>& buffer) {
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        inl::log::error("Failed to open file: {}", path.c_str());
    }

    std::size_t file_size = std::filesystem::file_size(path);
    buffer.resize(file_size);
    file.read(reinterpret_cast<char*>(buffer.data()), file_size);
}

// TODO: Pull these into a ResourceManager
std::optional<inl::Texture> load_texture(std::filesystem::path path, bool flip_vertically) {

    using namespace inl;

    std::vector<std::uint8_t> raw_image_data {};
    auto start_image = std::chrono::steady_clock::now();
    read_file(path, raw_image_data);
    log::debug("Image size: {}", raw_image_data.size());

    ppm::Result<ppm::Image> image_or_error = ppm::load(raw_image_data);
    if (!image_or_error) {
        log::error("Failed to load ppm image error: {}", static_cast<int>(image_or_error.error()));
        return std::nullopt;
    }

    auto end_image = std::chrono::steady_clock::now();
    log::debug("PPM image: f:{}, w:{}, h:{}, v:{}, size:{}, time:{} ", static_cast<int>(image_or_error->format),
        image_or_error->width, image_or_error->height, image_or_error->max_value, image_or_error->pixel_data.size(),
        std::chrono::duration_cast<std::chrono::milliseconds>(end_image - start_image));

    ppm::Image image;
    if (flip_vertically) {
        image = ppm::flip_vertically(*image_or_error);
    } else {
        image = *image_or_error;
    }

    Texture texture { image.width, image.height, 3, image.pixel_data.data() };

    glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

// std::optional<std::array<inl::Texture, 6>> load_skybox(std::array<std::string_view, 6> paths, bool flip_vertically) {
//
//     using namespace inl;
//
//     for (std::size_t i = 0; i < paths.size(); ++i) {
//         std::vector<std::uint8_t> raw_image_data {};
//         read_file(paths[i], raw_image_data);
//         log::debug("Image size: {}", raw_image_data.size());
//
//         ppm::Result<ppm::Image> image_or_error = ppm::load(raw_image_data);
//         if (!image_or_error) {
//             log::error("Failed to load ppm image error: {}", static_cast<int>(image_or_error.error()));
//             return std::nullopt;
//         }
//
//         log::debug("PPM image: f:{}, w:{}, h:{}, v:{}, size:{} ", static_cast<int>(image_or_error->format),
//             image_or_error->width, image_or_error->height, image_or_error->max_value,
//             image_or_error->pixel_data.size());
//
//         ppm::Image image;
//         if (flip_vertically) {
//             image = ppm::flip_vertically(*image_or_error);
//         } else {
//             image = *image_or_error;
//         }
//
//         Texture texture { image.width, image.height, 3, image.pixel_data.data() };
//     }
//
//     glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTextureParameteri(texture.native_handle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     glTextureParameteri(texture.native_handle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//     return texture;
// }

} // namespace inl
