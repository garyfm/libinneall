#include <libinneall/asset/ppm.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/texture.hpp>

#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

namespace inl {

// TODO: These are quick and dirty implementaitons
// Some kind of Asset manager should be used
std::string read_file(std::filesystem::path path);
void read_file(std::filesystem::path path, std::vector<std::uint8_t>& buffer);
std::optional<inl::Texture> load_texture(std::filesystem::path path, bool flip_vertically);
std::optional<std::array<ppm::Image, 6>> load_cubemap(std::array<std::string, 6> paths, bool flip_vertically);
std::optional<inl::ShaderProgram> load_shader(
    std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);

} // namespace inl
