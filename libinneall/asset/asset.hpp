#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/array.hpp>
#include <libinneall/base/option.hpp>
#include <libinneall/base/result.hpp>
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

Option<ByteSpan> load_file(ByteSpan buffer, std::filesystem::path path);
Option<StringView> load_text_file(Span<uint8_t> buffer, std::filesystem::path path);
Option<ppm::Image> load_image(ByteSpan buffer, std::filesystem::path path);

Error load_texture(ByteSpan buffer, Texture& texture, std::filesystem::path path, bool flip_vertically);
Error load_cubemap(ByteSpan buffer, Cubemap& cubemap, StringView path, bool flip_vertically);
Error load_shader(ByteSpan buffer, ShaderProgram& shader_program, std::filesystem::path vertex_shader_path,
    std::filesystem::path fragment_shader_path);
Error load_mesh(ByteSpan buffer, Mesh& mesh, std::filesystem::path path);

} // namespace inl
