#include "asset/ppm.hpp"
#include <libinneall/asset/asset.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/renderer/shader_stage.hpp>
#include <libinneall/renderer/texture.hpp>

#include <fstream>

namespace inl {

Option<ByteSpan> load_file(ByteSpan buffer, std::filesystem::path path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        log_error("Failed to open file: {}", path.c_str());
        return None;
    }

    size_t file_size = std::filesystem::file_size(path);

    if (file_size == 0) {
        log_error("File is empty");
        return ByteSpan {};
    }

    if (file_size > buffer.size()) {
        log_error("File ({}) wont fit in buffer ()", file_size, buffer.size());
        return None;
    }

    file.read(reinterpret_cast<char*>(buffer.data()), file_size);

    log_info("file_size {}", file_size);
    return ByteSpan { buffer.data(), file_size };
}

Option<StringView> load_text_file(ByteSpan buffer, std::filesystem::path path) {

    Option<ByteSpan> raw_file_data = load_file(buffer, path);
    if (!raw_file_data) {
        log_error("Failed to load file");
        return None;
    }
    return StringView { reinterpret_cast<char const*>(raw_file_data->data()), raw_file_data->size() };
}

Error load_image(ByteSpan buffer, ppm::Image& image, std::filesystem::path path) {
    Option<ByteSpan> raw_image_data { load_file(buffer, path) };

    if (!raw_image_data) {
        return Error::AssetFailedToLoadFile;
    }

    TRY(ppm::load(image, *raw_image_data));

    log_debug("PPM image: f:{}, w:{}, h:{}, v:{}, size:{}", static_cast<int32_t>(image.format), image.width,
        image.height, image.max_value, image.pixel_data.size());

    return Error::Ok;
}

Error load_texture(ByteSpan buffer, Texture& texture, std::filesystem::path path, bool flip_vertically) {

    log_info("Loading texture: {}", path.filename().c_str());

    ppm::Image image {};
    TRY(load_image(buffer, image, path));

    // TODO:: Buffer needs to be twice the size of image to flip, not ideal
    if (flip_vertically) {
        ppm::flip_vertically({ &buffer[image.size_bytes()], image.size_bytes() }, image);
    }

    Error error = Texture::create(texture, image.width, image.height, 3, image.pixel_data.data());

    if (error != Error::Ok) {
        return error;
    }

    glTextureParameteri(texture.handle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.handle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.handle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture.handle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

        log_info("Loading texture: {}", path[i]);

        if (i == 0) {
            image_path.append(cubemap_files[i]);
        } else {
            image_path.overwrite(cubemap_files[i], path.size());
        }

        ppm::Image image {};
        TRY(load_image({ &buffer[cursor_pos], buffer.size() }, image, image_path.data()));

        cursor_pos += image.size_bytes();

        if (flip_vertically) {
            ppm::flip_vertically(buffer, image);
        }

        cubemap_data[i] = image;
    }

    Array<uint8_t const*, 6> skybox_faces { {
        cubemap_data[0].pixel_data.data(),
        cubemap_data[1].pixel_data.data(),
        cubemap_data[2].pixel_data.data(),
        cubemap_data[3].pixel_data.data(),
        cubemap_data[4].pixel_data.data(),
        cubemap_data[5].pixel_data.data(),
    } };

    return Cubemap::create(cubemap, cubemap_data[0].width, cubemap_data[0].height, 3, skybox_faces);
}

Error load_shader(ByteSpan buffer, ShaderProgram& shader_program, std::filesystem::path vertex_shader_path,
    std::filesystem::path fragment_shader_path) {

    log_info("Loading shader: {}", vertex_shader_path.filename().c_str());

    Option<StringView> vertex_shader_source = load_text_file(buffer, vertex_shader_path);
    if (!vertex_shader_source) {
        log_error("Failed to load shader stage: {}", vertex_shader_path.filename().c_str());
        return Error::AssetFailedToLoadShader;
    }
    log_info("Loading shader: {}", fragment_shader_path.filename().c_str());
    ShaderStage vertex_stage {};
    TRY(ShaderStage::create(vertex_stage, ShaderType::Vertex, *vertex_shader_source));

    Option<StringView> fragment_shader_source = load_text_file(buffer, fragment_shader_path);
    if (!fragment_shader_source) {
        log_error("Failed to load shader stage: {}", fragment_shader_path.filename().c_str());
        return Error::AssetFailedToLoadShader;
    }

    ShaderStage fragment_stage {};
    TRY(ShaderStage::create(fragment_stage, ShaderType::Fragment, *fragment_shader_source));

    return ShaderProgram::create(shader_program, vertex_stage, fragment_stage);
}

Error load_mesh(ByteSpan buffer, Mesh& mesh, std::filesystem::path path) {

    log_info("Loading mesh: {}", path.filename().c_str());

    Option<StringView> obj_data { load_text_file(buffer, path) };
    if (!obj_data) {
        return Error::AssetFailedToLoadMesh;
    }

    obj::Model obj_model {};
    TRY(obj::load(obj_model, { obj_data->data(), obj_data->size() }));

    log_debug("OBJ model: vertices:{}, textures:{}, normals:{}, faces:{}", obj_model.geometric_vertices.size(),
        obj_model.texture_vertices.size(), obj_model.vertex_normals.size(), obj_model.face_corners.size());

    MeshData mesh_data = to_mesh_data(obj_model);

    return Mesh::create(mesh, mesh_data);
}

} // namespace inl
