#include <libinneall/asset/asset.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/renderer/shader_stage.hpp>
#include <libinneall/renderer/texture.hpp>

#include <fstream>
#include <optional>
#include <span>

namespace inl {

std::optional<ByteBuffer> load_file(std::filesystem::path path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        log::error("Failed to open file: {}", path.c_str());
        return std::nullopt;
    }

    size_t file_size = std::filesystem::file_size(path);

    if (file_size == 0) {
        return ByteBuffer {};
    }

    ByteBuffer contents(file_size);
    file.read(reinterpret_cast<char*>(contents.data()), file_size);

    return contents;
}

std::optional<std::string> load_text_file(std::filesystem::path path) {

    std::optional<ByteBuffer> byte_buffer = load_file(path);
    if (!byte_buffer) {
        return std::nullopt;
    }

    std::string contents { reinterpret_cast<const char*>(byte_buffer->data()), byte_buffer->size() };

    return contents;
}

std::optional<ppm::Image> load_image(std::filesystem::path path) {
    std::optional<ByteBuffer> raw_image_data { load_file(path) };
    if (!raw_image_data) {
        return std::nullopt;
    }

    ppm::Result<ppm::Image> image = ppm::load(*raw_image_data);
    if (!image) {
        log::error("Failed to load ppm image error: {}", static_cast<int32_t>(image.error()));
        return std::nullopt;
    }

    log::debug("PPM image: f:{}, w:{}, h:{}, v:{}, size:{}", static_cast<int32_t>(image->format), image->width,
        image->height, image->max_value, image->pixel_data.size());

    return *image;
}

std::optional<inl::Texture> load_texture(std::filesystem::path path, bool flip_vertically) {

    log::info("Loading texture: {}", path.filename().c_str());

    std::optional<ppm::Image> image { load_image(path) };
    if (!image) {
        return std::nullopt;
    }

    if (flip_vertically) {
        image = ppm::flip_vertically(*image);
    } else {
        image = *image;
    }

    Texture texture { image->width, image->height, 3, image->pixel_data.data() };

    glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

std::optional<Cubemap> load_cubemap(Array<std::string, 6> paths, bool flip_vertically) {

    Array<ppm::Image, 6> cubemap_data {};

    for (size_t i = 0; i < cubemap_data.size(); ++i) {

        log::info("Loading texture: {}", paths[i]);

        std::optional<ppm::Image> image { load_image(paths[i]) };
        if (!image) {
            return std::nullopt;
        }

        if (flip_vertically) {
            image = ppm::flip_vertically(*image);
        } else {
            image = *image;
        }

        cubemap_data[i] = *image;
    }

    Array<uint8_t const*, 6> skybox_faces { {
        cubemap_data[0].pixel_data.data(),
        cubemap_data[1].pixel_data.data(),
        cubemap_data[2].pixel_data.data(),
        cubemap_data[3].pixel_data.data(),
        cubemap_data[4].pixel_data.data(),
        cubemap_data[5].pixel_data.data(),
    } };

    Cubemap cubemap { cubemap_data[0].width, cubemap_data[0].height, 3, skybox_faces };

    return cubemap;
}

std::optional<inl::ShaderProgram> load_shader(
    std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path) {
    std::string_view sv_path { vertex_shader_path.c_str() };
    std::string_view::size_type name_start_pos = sv_path.find_last_of("/") + 1;
    std::string_view::size_type name_end_pos = sv_path.find(".", name_start_pos);
    std::string_view shader_name = sv_path.substr(name_start_pos, name_end_pos - name_start_pos);

    log::info("Loading shader program: {}", shader_name);

    std::optional<std::string> vertex_source { load_text_file(vertex_shader_path) };
    if (!vertex_source) {
        log::error("Failed to load shader stage: {}", vertex_shader_path.filename().c_str());
        return std::nullopt;
    }

    std::optional<std::string> fragment_source { load_text_file(fragment_shader_path) };
    if (!fragment_source) {
        log::error("Failed to load shader stage: {}", vertex_shader_path.filename().c_str());
        return std::nullopt;
    }

    ShaderStage vertex_stage { ShaderType::Vertex, *vertex_source };
    ShaderStage fragment_stage { ShaderType::Fragment, *fragment_source };

    ShaderProgram shader_program { std::move(vertex_stage), std::move(fragment_stage) };

    return shader_program;
}

std::optional<inl::Mesh> load_mesh(std::filesystem::path path) {

    log::info("Loading mesh: {}", path.filename().c_str());

    std::optional<std::string> obj_data { load_text_file(path) };
    if (!obj_data) {
        return std::nullopt;
    }

    obj::Result<obj::Model> obj_model = obj::load(*obj_data);
    if (!obj_model) {
        log::error("Failed to load obj file error: {}", static_cast<int32_t>(obj_model.error()));
        return std::nullopt;
    }

    log::debug("OBJ model: vertices:{}, textures:{}, normals:{}, faces:{}", obj_model->geometric_vertices.size(),
        obj_model->texture_vertices.size(), obj_model->vertex_normals.size(), obj_model->face_corners.size());

    MeshData mesh_data = to_mesh_data(*obj_model);

    Mesh mesh { mesh_data };

    return mesh;
}

} // namespace inl
