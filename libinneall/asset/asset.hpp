#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/arena.hpp>
#include <libinneall/base/array.hpp>
#include <libinneall/base/error.hpp>
#include <libinneall/base/option.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/base/string_view.hpp>
#include <libinneall/renderer/cubemap.hpp>
#include <libinneall/renderer/material.hpp>
#include <libinneall/renderer/mesh.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/texture.hpp>

#include <filesystem>
#include <vector>

namespace inl {

constexpr size_t MAX_ASSET_PATH_SIZE { 128 };
using ByteBuffer = std::vector<uint8_t>;

// TODO: Some kind of Asset manager should be used

Error load_file(Arena* arena, ByteSpan& file_data, std::filesystem::path path);
Error load_text_file(Arena& arena, ByteSpan& file_data, std::filesystem::path path);
Error load_image(Arena& arena, ppm::Image& image, std::filesystem::path path);
Error load_texture(Arena& arena, Texture& texture, std::filesystem::path path, bool flip_vertically);
Error load_cubemap(Arena& arena, Cubemap& cubemap, StringView path, bool flip_vertically);
Error load_shader(Arena& arena, ShaderProgram& shader_program, std::filesystem::path vertex_shader_path,
    std::filesystem::path fragment_shader_path);
Error load_mesh(Arena& arena, Mesh& mesh, std::filesystem::path path);

} // namespace inl
