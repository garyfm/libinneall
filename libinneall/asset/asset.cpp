#include <libinneall/asset/asset.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/renderer/shader_stage.hpp>
#include <libinneall/renderer/texture.hpp>

#include <fstream>

namespace inl {

Option<ByteSpan> load_file(ByteSpan buffer, std::filesystem::path path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        log::error("Failed to open file: {}", path.c_str());
        return None;
    }

    size_t file_size = std::filesystem::file_size(path);

    if (file_size == 0) {
        log::error("File is empty");
        return ByteSpan {};
    }

    if (file_size > buffer.size()) {
        log::error("File ({}) wont fit in buffer ()", file_size, buffer.size());
        return None;
    }

    file.read(reinterpret_cast<char*>(buffer.data()), file_size);

    log::info("file_size {}", file_size);
    return ByteSpan { buffer.data(), file_size };
}

Option<StringView> load_text_file(ByteSpan buffer, std::filesystem::path path) {

    Option<ByteSpan> raw_file_data = load_file(buffer, path);
    if (!raw_file_data) {
        log::error("Failed to load file");
        return None;
    }
    return StringView { reinterpret_cast<char const*>(raw_file_data->data()), raw_file_data->size() };
}

Option<ppm::Image> load_image(ByteSpan buffer, std::filesystem::path path) {
    Option<ByteSpan> raw_image_data { load_file(buffer, path) };

    if (!raw_image_data) {
        log::error("Failed to load file");
        return None;
    }

    ppm::Result<ppm::Image> image { ppm::load(*raw_image_data) };
    if (!image) {
        log::error("Failed to load ppm image error: {}", static_cast<int32_t>(image.error()));
        return None;
    }

    log::debug("PPM image: f:{}, w:{}, h:{}, v:{}, size:{}", static_cast<int32_t>(image->format), image->width,
        image->height, image->max_value, image->pixel_data.size());

    return *image;
}

Error load_texture(ByteSpan buffer, Texture& texture, std::filesystem::path path, bool flip_vertically) {

    log::info("Loading texture: {}", path.filename().c_str());

    Option<ppm::Image> image { load_image(buffer, path) };
    if (!image) {
        return Error::AssetFailedToLoadTexture;
    }

    // TODO:: Buffer needs to be twice the size of image to flip, not ideal
    if (flip_vertically) {
        image = ppm::flip_vertically({ &buffer[image->size_bytes()], image->size_bytes() }, *image);
    } else {
        image = *image;
    }

    texture.create(image->width, image->height, 3, image->pixel_data.data());

    glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return Error::Ok;
}

Error load_cubemap(ByteSpan buffer, Cubemap& cubemap, StringView path, bool flip_vertically) {

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

    size_t cursor_pos { 0 };
    for (size_t i = 0; i < cubemap_data.size(); ++i) {

        log::info("Loading texture: {}", path[i]);

        if (i == 0) {
            image_path.append(cubemap_files[i]);
        } else {
            image_path.overwrite(cubemap_files[i], path.size());
        }

        Option<ppm::Image> image { load_image({ &buffer[cursor_pos], buffer.size() }, image_path.data()) };
        if (!image) {
            return Error::AssetFailedToLoadCubmap;
        }

        cursor_pos += image->size_bytes();

        if (flip_vertically) {
            image = ppm::flip_vertically(buffer, *image);
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

    cubemap.create(cubemap_data[0].width, cubemap_data[0].height, 3, skybox_faces);

    return Error::Ok;
}

Error load_shader(ByteSpan buffer, ShaderProgram& shader_program, std::filesystem::path vertex_shader_path,
    std::filesystem::path fragment_shader_path) {

    log::info("Loading shader: {}", vertex_shader_path.filename().c_str());

    Option<StringView> vertex_shader_source = load_text_file(buffer, vertex_shader_path);
    if (!vertex_shader_source) {
        log::error("Failed to load shader stage: {}", vertex_shader_path.filename().c_str());
        return Error::AssetFailedToLoadShader;
    }
    log::info("Loading shader: {}", fragment_shader_path.filename().c_str());
    static ShaderStage vertex_stage {};
    vertex_stage.create(ShaderType::Vertex, *vertex_shader_source);

    Option<StringView> fragment_shader_source = load_text_file(buffer, fragment_shader_path);
    if (!fragment_shader_source) {
        log::error("Failed to load shader stage: {}", fragment_shader_path.filename().c_str());
        return Error::AssetFailedToLoadShader;
    }
    static ShaderStage fragment_stage {};
    fragment_stage.create(ShaderType::Fragment, *fragment_shader_source);

    shader_program.create(vertex_stage, fragment_stage);

    return Error::Ok;
}

Error load_mesh(ByteSpan buffer, Mesh& mesh, std::filesystem::path path) {

    log::info("Loading mesh: {}", path.filename().c_str());

    Option<StringView> obj_data { load_text_file(buffer, path) };
    if (!obj_data) {
        return Error::AssetFailedToLoadMesh;
    }

    obj::Result<obj::Model> obj_model = obj::load({ obj_data->data(), obj_data->size() });
    if (!obj_model) {
        log::error("Failed to load obj file error: {}", static_cast<int32_t>(obj_model.error()));
        return Error::AssetFailedToLoadMesh;
    }

    log::debug("OBJ model: vertices:{}, textures:{}, normals:{}, faces:{}", obj_model->geometric_vertices.size(),
        obj_model->texture_vertices.size(), obj_model->vertex_normals.size(), obj_model->face_corners.size());

    static MeshData mesh_data = to_mesh_data(*obj_model);

    mesh.create(mesh_data);

    return Error::Ok;
}

} // namespace inl
