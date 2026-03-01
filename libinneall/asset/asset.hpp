#include <filesystem>
#include <libinneall/renderer/texture.hpp>
#include <optional>
#include <string_view>
#include <vector>

namespace inl {

std::string read_file(std::filesystem::path path);
void read_file(std::filesystem::path path, std::vector<std::uint8_t>& buffer);
std::optional<inl::Texture> load_texture(std::filesystem::path path, bool flip_vertically);
std::optional<std::array<inl::Texture, 6>> load_skybox(std::array<std::string_view, 6> paths, bool flip_vertically);

} // namespace inl
