#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/array.hpp>
#include <libinneall/renderer/cubemap.hpp>
#include <libinneall/renderer/material.hpp>
#include <libinneall/renderer/mesh.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/texture.hpp>

#include <filesystem>
#include <optional>
#include <vector>

namespace inl {

// TODO: Some kind of Asset manager should be used
using ByteBuffer = std::vector<uint8_t>;

std::optional<ByteBuffer> load_file(std::filesystem::path path);
std::optional<std::string> load_text_file(std::filesystem::path path);
std::optional<ppm::Image> load_image(std::filesystem::path path);
std::optional<inl::Texture> load_texture(std::filesystem::path path, bool flip_vertically);
std::optional<Cubemap> load_cubemap(Array<std::string, 6> paths, bool flip_vertically);
std::optional<inl::ShaderProgram> load_shader(
    std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);
std::optional<inl::Mesh> load_mesh(std::filesystem::path path);

} // namespace inl
