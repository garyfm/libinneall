#include <libinneall/asset/obj.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/buffer.hpp>
#include <libinneall/base/error.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/string.hpp>
#include <libinneall/base/string_view.hpp>

#include <errno.h>
#include <stdlib.h>

namespace {

constexpr inl::StringView DATA_TYPE_GEOMETRIC_VERTICES = "v";
constexpr inl::StringView DATA_TYPE_TEXTURE_VERTICES = "vt";
constexpr inl::StringView DATA_TYPE_VERTEX_NORMALS = "vn";
constexpr inl::StringView DATA_TYPE_FACE = "f";

constexpr inl::StringView DATA_TYPE_MTL = "mtllib";
constexpr inl::StringView DATA_TYPE_MTL_NAME = "newmtl";
constexpr inl::StringView DATA_TYPE_MTL_KA = "Ka";
constexpr inl::StringView DATA_TYPE_MTL_KD = "Kd";
constexpr inl::StringView DATA_TYPE_MTL_KS = "Ks";
constexpr inl::StringView DATA_TYPE_MTL_NS = "Ns";
constexpr inl::StringView DATA_TYPE_MTL_D = "d";
constexpr inl::StringView DATA_TYPE_MTL_MAP_KD = "map_Kd";
constexpr inl::StringView DATA_TYPE_MTL_MAP_KN = "map_Kn";
constexpr inl::StringView DATA_TYPE_MTL_MAP_KS = "map_Ks";

inl::Error extract_float(float& value, inl::StringView obj_data) {

    char* end;
    errno = 0;
    // TODO:: Implement a to_float funtion
    // strtof assumes NULL terminate which StringView is not
    float result = strtof(obj_data.data(), &end);

    if (end == obj_data.data()) return inl::Error::ObjFailedToExtractFloat;

    if (errno == ERANGE) return inl::Error::ObjFailedToExtractFloat;

    value = result;

    return inl::Error::Ok;
}

uint32_t map_index(size_t current_index, float index) {
    if (index < 0) {
        // handle negative index
        return static_cast<uint32_t>(current_index + static_cast<uint32_t>(index) + 1);
    } else {
        return static_cast<uint32_t>(index);
    }
}

inl::Error parse_vector3(inl::Vector3& vector, inl::StringView data_values) {
    inl::Cut cut_vertex = inl::cut(data_values, ' ');

    TRY(extract_float(vector.x, cut_vertex.left));

    cut_vertex = inl::cut(inl::trim(cut_vertex.right), ' ');
    TRY(extract_float(vector.y, cut_vertex.left));

    TRY(extract_float(vector.z, cut_vertex.right));
    return inl::Error::Ok;
}

struct ModelCount {
    size_t n_verts {};
    size_t n_texts {};
    size_t n_norms {};
    size_t n_faces {};
};

ModelCount model_count(inl::StringView const obj_data) {
    ModelCount count {};

    inl::Cut lines {};
    lines.right = obj_data;

    while (lines.right.size()) {
        lines = cut(lines.right, '\n');

        inl::StringView line = trim(lines.left);
        if (line.empty()) {
            continue;
        }

        inl::Cut cut_data = cut(line, ' ');

        inl::StringView data_type = trim(cut_data.left);

        if (data_type == DATA_TYPE_GEOMETRIC_VERTICES) {
            ++count.n_verts;
        } else if (data_type == DATA_TYPE_TEXTURE_VERTICES) {
            ++count.n_texts;
        } else if (data_type == DATA_TYPE_VERTEX_NORMALS) {
            ++count.n_norms;
        } else if (data_type == DATA_TYPE_FACE) {
            ++count.n_faces;
        }
    }

    return count;
}

}

namespace inl::obj {

Error load(Arena& arena, Mesh& mesh, StringView obj_data) {

    ModelCount count = model_count(obj_data);

    Buffer<Vector3> geometric_vertices { arena, count.n_verts };
    Buffer<Vector2> texture_vertices { arena, count.n_texts };
    Buffer<Vector3> vertex_normals { arena, count.n_norms };
    static constexpr size_t N_CORNERS_PER_FACE = 3;
    Buffer<FaceCorner> face_corners = { arena, count.n_faces * N_CORNERS_PER_FACE };

    inl::Cut lines {};
    lines.right = obj_data;

    while (lines.right.size()) {
        lines = cut(lines.right, '\n');

        if (!lines.success) {
            return Error::ObjInvalidFormat;
        }

        StringView line = trim(lines.left);
        if (line.empty()) {
            continue;
        }

        Cut cut_data = cut(line, ' ');

        StringView data_type = trim(cut_data.left);
        StringView data_values = trim(cut_data.right);

        if (data_type == DATA_TYPE_GEOMETRIC_VERTICES) {
            Vector3 vector {};
            TRY(parse_vector3(vector, data_values));
            geometric_vertices.push(vector);
        } else if (data_type == DATA_TYPE_TEXTURE_VERTICES) {
            Vector2 vertex_texture {};
            Cut cut_vertex_texture = cut(data_values, ' ');

            TRY(extract_float(vertex_texture.x, cut_vertex_texture.left));
            TRY(extract_float(vertex_texture.y, cut_vertex_texture.right));

            texture_vertices.push(vertex_texture);
        } else if (data_type == DATA_TYPE_VERTEX_NORMALS) {
            Vector3 vector {};
            TRY(parse_vector3(vector, data_values));
            vertex_normals.push(vector);
        } else if (data_type == DATA_TYPE_FACE) {
            Cut cut_face { .left = {}, .right = data_values, .success = true };
            while (cut_face.right.size() > 0) {
                cut_face = cut(trim(cut_face.right), ' ');
                Cut cut_index = cut(cut_face.left, '/');

                FaceCorner corner {};
                float vertex_index {};
                TRY(extract_float(vertex_index, cut_index.left));
                corner.vertex_index = map_index(geometric_vertices.size(), vertex_index);
                inl_assert(corner.vertex_index - 1 < geometric_vertices.size(), "Invalid obj vertex index");

                // Cut successful, process next index
                if (cut_index.success) {
                    cut_index = cut(cut_index.right, '/');

                    float texture_index {};
                    TRY(extract_float(texture_index, cut_index.left));
                    corner.texture_index = map_index(texture_vertices.size(), texture_index);
                    inl_assert(corner.texture_index - 1 < texture_vertices.size(), "Invalid obj texture index");

                    // Cut successfull, process next index
                    if (cut_index.success) {
                        float normal_index {};
                        TRY(extract_float(normal_index, cut_index.right));
                        corner.normal_index = map_index(vertex_normals.size(), normal_index);
                        inl_assert(corner.normal_index - 1 < vertex_normals.size(), "Invalid obj normal index");
                    }
                }

                face_corners.push(corner);
            }
        }
    }

    mesh.geometric_vertices = geometric_vertices;
    mesh.texture_vertices = texture_vertices;
    mesh.vertex_normals = vertex_normals;
    mesh.face_corners = face_corners;

    return Error::Ok;
}

Error load(Mtl& mtl, StringView mtl_data) {

    inl::Cut lines {};
    lines.right = mtl_data;

    while (lines.right.size()) {
        lines = cut(lines.right, '\n');

        if (!lines.success) {
            return Error::ObjInvalidFormat;
        }

        StringView line = trim(lines.left);
        if (line.empty()) {
            continue;
        }

        Cut cut_data = cut(line, ' ');

        StringView data_type = trim(cut_data.left);
        StringView data_values = trim(cut_data.right);

        if (data_type == DATA_TYPE_MTL_NAME) {
            mtl.name = data_values;
            log_debug("mtl.name: " FMT_SV, FMT_SV_ARGS(mtl.name));
        } else if (data_type == DATA_TYPE_MTL_KA) {
            TRY(parse_vector3(mtl.ambient, data_values));
            log_debug("mtl.ambient: " FMT_VEC3, FMT_VEC3_ARGS(mtl.ambient));
        } else if (data_type == DATA_TYPE_MTL_KD) {
            TRY(parse_vector3(mtl.diffuse, data_values));
            log_debug("mtl.diffuse: " FMT_VEC3, FMT_VEC3_ARGS(mtl.diffuse));
        } else if (data_type == DATA_TYPE_MTL_KS) {
            TRY(parse_vector3(mtl.specular, data_values));
            log_debug("mtl.specular: " FMT_VEC3, FMT_VEC3_ARGS(mtl.specular));
        } else if (data_type == DATA_TYPE_MTL_NS) {
            TRY(extract_float(mtl.shininess, data_values));
            log_debug("mtl.shininess: %f", mtl.shininess);
        } else if (data_type == DATA_TYPE_MTL_D) {
            TRY(extract_float(mtl.dissolve, data_values));
            log_debug("mtl.dissolve: %f", mtl.dissolve);
        } else if (data_type == DATA_TYPE_MTL_MAP_KD) {
            mtl.map_albedo = data_values;
            log_debug("mtl.map_albedo: " FMT_SV, FMT_SV_ARGS(mtl.map_albedo));
        } else if (data_type == DATA_TYPE_MTL_MAP_KN) {
            mtl.map_normal = data_values;
            log_debug("mtl.map_normal: " FMT_SV, FMT_SV_ARGS(mtl.map_normal));
        } else if (data_type == DATA_TYPE_MTL_MAP_KS) {
            mtl.map_specular = data_values;
            log_debug("mtl.map_specular: " FMT_SV, FMT_SV_ARGS(mtl.map_specular));
        }
    }

    return Error::Ok;
}

} // namespace inl
