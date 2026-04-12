#include <libinneall/asset/asset.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/renderer/shader_stage.hpp>
#include <libinneall/renderer/texture.hpp>

#include <fstream>

namespace inl {

Option<ByteBuffer> load_file(std::filesystem::path path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        log::error("Failed to open file: {}", path.c_str());
        return None;
    }

    size_t file_size = std::filesystem::file_size(path);

    if (file_size == 0) {
        return ByteBuffer {};
    }

    ByteBuffer contents(file_size);
    file.read(reinterpret_cast<char*>(contents.data()), file_size);

    return contents;
}

Option<StringView> load_text_file(std::filesystem::path path, Span<uint8_t> buffer) {

    // TODO: This should take the buffer in
    Option<ByteBuffer> byte_buffer = load_file(path);
    if (!byte_buffer) {
        return None;
    }
    // TODO: Read directly into buffer rather than copy
    memcpy(buffer.data(), byte_buffer->data(), byte_buffer->size());
    return StringView { reinterpret_cast<char const*>(buffer.data()), byte_buffer->size() };
}

Option<ppm::Image> load_image(std::filesystem::path path) {
    Option<ByteBuffer> raw_image_data { load_file(path) };
    if (!raw_image_data) {
        return None;
    }

    ppm::Result<ppm::Image> image = ppm::load({ raw_image_data->data(), raw_image_data->size() });
    if (!image) {
        log::error("Failed to load ppm image error: {}", static_cast<int32_t>(image.error()));
        return None;
    }

    log::debug("PPM image: f:{}, w:{}, h:{}, v:{}, size:{}", static_cast<int32_t>(image->format), image->width,
        image->height, image->max_value, image->pixel_data.size());

    return *image;
}

Option<inl::Texture> load_texture(std::filesystem::path path, bool flip_vertically) {

    log::info("Loading texture: {}", path.filename().c_str());

    Option<ppm::Image> image { load_image(path) };
    if (!image) {
        return None;
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

Option<Cubemap> load_cubemap(StringView path, bool flip_vertically) {

    Array<ppm::Image, 6> cubemap_data {};
    const Array<StringView, 6> cubemap_files { {
        { "/right.ppm" },
        { "/left.ppm" },
        { "/top.ppm" },
        { "/bottom.ppm" },
        { "/front.ppm" },
        { "/back.ppm" },
    } };

    String<MAX_ASSET_PATH_SIZE> image_path { path };

    for (size_t i = 0; i < cubemap_data.size(); ++i) {

        log::info("Loading texture: {}", path[i]);

        if (i == 0) {
            image_path.append(cubemap_files[i]);
        } else {
            image_path.overwrite(cubemap_files[i], path.size());
        }

        Option<ppm::Image> image { load_image(image_path.data()) };
        if (!image) {
            return None;
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

Option<inl::ShaderProgram> load_shader(
    std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path, Span<uint8_t> buffer) {

    log::info("Loading shader: {}", vertex_shader_path.filename().c_str());

    Option<StringView> vertex_shader_source = load_text_file(vertex_shader_path, buffer);
    if (!vertex_shader_source) {
        log::error("Failed to load shader stage: {}", vertex_shader_path.filename().c_str());
        return None;
    }

    log::info("Loading shader: {}", fragment_shader_path.filename().c_str());
    ShaderStage vertex_stage { ShaderType::Vertex, *vertex_shader_source };
    Option<StringView> fragment_shader_source = load_text_file(fragment_shader_path, buffer);
    if (!fragment_shader_source) {
        log::error("Failed to load shader stage: {}", fragment_shader_path.filename().c_str());
        return None;
    }

    ShaderStage fragment_stage { ShaderType::Fragment, *fragment_shader_source };

    ShaderProgram shader_program { std::move(vertex_stage), std::move(fragment_stage) };

    return shader_program;
}

Option<inl::Mesh> load_mesh(std::filesystem::path path, Span<uint8_t> buffer) {

    log::info("Loading mesh: {}", path.filename().c_str());

    Option<StringView> obj_data { load_text_file(path, buffer) };
    if (!obj_data) {
        return None;
    }

    obj::Result<obj::Model> obj_model = obj::load({ obj_data->data(), obj_data->size() });
    if (!obj_model) {
        log::error("Failed to load obj file error: {}", static_cast<int32_t>(obj_model.error()));
        return None;
    }

    log::debug("OBJ model: vertices:{}, textures:{}, normals:{}, faces:{}", obj_model->geometric_vertices.size(),
        obj_model->texture_vertices.size(), obj_model->vertex_normals.size(), obj_model->face_corners.size());

    MeshData mesh_data = to_mesh_data(*obj_model);

    Mesh mesh { mesh_data };

    return mesh;
}

} // namespace inl
