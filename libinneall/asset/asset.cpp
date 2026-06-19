#include <libinneall/asset/asset.hpp>
#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/renderer/shader_stage.hpp>
#include <libinneall/renderer/texture.hpp>

#include <stdio.h>

namespace inl {

Error load_file(Arena& arena, ByteSpan& file_data, StringView path) {

    FILE* file = fopen(path.data(), "rb");

    if (!file) {
        log_error("Failed to open file: {}", path.data());
        return Error::AssetFailedToOpenFile;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return Error::AssetFailedToSeekFile;
    }

    long size = ftell(file);
    if (size < 0) {
        fclose(file);
        return Error::AssetInvalidFileSize;
    }

    size_t file_size = static_cast<size_t>(size);
    inl_assert(file_size < arena.capacity(), "File exceeds arena capacity");

    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return Error::AssetFailedToSeekFile;
    }

    ArenaMark arena_mark = arena.mark();
    uint8_t* buffer = arena.alloc_array<uint8_t>(file_size ? file_size : 1);

    size_t bytes_read = fread(buffer, sizeof(uint8_t), file_size, file);

    if (bytes_read != file_size) {
        fclose(file);
        arena.reset_to(arena_mark);
        return Error::AssetFailedToReadFile;
    }

    fclose(file);

    file_data = ByteSpan { buffer, file_size };

    return Error::Ok;
}

Error load_text_file(Arena& arena, StringView& file_data, StringView path) {

    ByteSpan raw_data {};
    TRY(load_file(arena, raw_data, path));
    file_data = StringView { reinterpret_cast<char*>(raw_data.data()), raw_data.size() };
    return Error::Ok;
}

Error load_image(Arena& arena, ppm::Image& image, StringView path) {
    ByteSpan raw_image_data {};
    TRY(load_file(arena, raw_image_data, path));
    TRY(ppm::load(image, raw_image_data));

    log_debug("PPM image: f:%d, w:%d, h:%d, v:%d size:%d", static_cast<int32_t>(image.format), image.width,
        image.height, image.max_value, image.pixel_data.size());

    return Error::Ok;
}

Error load_texture(Arena& arena, Texture& texture, StringView path, bool flip_vertically) {
    log_info("Loading texture: %s", path.data());

    ppm::Image image {};
    TRY(load_image(arena, image, path));

    if (flip_vertically) {
        ppm::flip_vertically(arena, image);
    }

    TRY(Texture::create(texture, image.width, image.height, 3, image.pixel_data.data()));

    glTextureParameteri(texture.handle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.handle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.handle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture.handle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return Error::Ok;
}

Error load_cubemap(Arena& arena, Cubemap& cubemap, StringView path, bool flip_vertically) {
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

        if (i == 0) {
            image_path.append(cubemap_files[i]);
        } else {
            image_path.overwrite(cubemap_files[i], path.size());
        }

        log_info("Loading texture: %s", image_path.data());

        ppm::Image image {};
        TRY(load_image(arena, image, image_path.data()));

        if (flip_vertically) {
            ppm::flip_vertically(arena, image);
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

Error load_shader(Arena& perm_arena, Arena& scratch_arena, ShaderProgram& shader_program, StringView vertex_shader_path,
    StringView fragment_shader_path) {
    log_info("Loading shader: %s", vertex_shader_path.data());

    StringView vertex_shader_source {};
    TRY(load_text_file(scratch_arena, vertex_shader_source, vertex_shader_path));

    log_info("Loading shader: %s", fragment_shader_path.data());
    ShaderStage vertex_stage {};
    TRY(ShaderStage::create(vertex_stage, ShaderType::Vertex, vertex_shader_source));

    StringView fragment_shader_source {};
    TRY(load_text_file(scratch_arena, fragment_shader_source, fragment_shader_path));

    ShaderStage fragment_stage {};
    TRY(ShaderStage::create(fragment_stage, ShaderType::Fragment, fragment_shader_source));

    return ShaderProgram::create(perm_arena, shader_program, vertex_stage, fragment_stage);
}

Error load_mesh(Arena& arena, Mesh& mesh, StringView path) {
    log_info("Loading mesh: %s", path.data());

    StringView obj_data {};
    TRY(load_text_file(arena, obj_data, path));

    obj::Model obj_model {};
    TRY(obj::load(arena, obj_model, { obj_data.data(), obj_data.size() }));

    log_debug("OBJ model: vertices:%u, textures:%u, normals:5u, faces:%u", obj_model.geometric_vertices.size(),
        obj_model.texture_vertices.size(), obj_model.vertex_normals.size(), obj_model.face_corners.size());

    // TODO: Free using temp
    MeshData mesh_data = to_mesh_data(arena, obj_model);

    return Mesh::create(mesh, mesh_data);
}

} // namespace inl
