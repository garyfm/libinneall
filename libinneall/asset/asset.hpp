#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/array.hpp>
#include <libinneall/base/option.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/renderer/cubemap.hpp>
#include <libinneall/renderer/material.hpp>
#include <libinneall/renderer/mesh.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/texture.hpp>

#include <filesystem>
#include <vector>

namespace inl {

constexpr size_t MAX_ASSET_PATH_SIZE { 128 };

// TODO: Some kind of Asset manager should be used
using ByteBuffer = std::vector<uint8_t>;

Option<ByteBuffer> load_file(std::filesystem::path path);
Option<StringView> load_text_file(std::filesystem::path path, Span<uint8_t> buffer);
Option<ppm::Image> load_image(std::filesystem::path path);
Option<inl::Texture> load_texture(std::filesystem::path path, bool flip_vertically);
Option<Cubemap> load_cubemap(StringView path, bool flip_vertically);
Option<inl::ShaderProgram> load_shader(
    std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path, Span<uint8_t> buffer);
Option<inl::Mesh> load_mesh(std::filesystem::path path, Span<uint8_t> buffer);

} // namespace inl
