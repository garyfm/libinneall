#include <libinneall/asset/obj.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/dyn_array.hpp>
#include <libinneall/base/error.hpp>
#include <libinneall/base/string.hpp>
#include <libinneall/base/string_view.hpp>

#include <errno.h>
#include <stdlib.h>

namespace {

inl::Error extract_float(float& value, inl::StringView obj_data) {

    char* end;
    errno = 0;
    // TODO:: Implement a to_float funtion
    // strtof assumes NULL terminate which StringView is not
    float result = strtof(obj_data.data(), &end);

    if (end == obj_data.data())
        return inl::Error::ObjFailedToExtractFloat;

    if (errno == ERANGE)
        return inl::Error::ObjFailedToExtractFloat;

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

constexpr inl::StringView DATA_TYPE_GEOMETRIC_VERTICES = "v";
constexpr inl::StringView DATA_TYPE_TEXTURE_VERTICES = "vt";
constexpr inl::StringView DATA_TYPE_VERTEX_NORMALS = "vn";
constexpr inl::StringView DATA_TYPE_FACE = "f";

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

Error load(Arena& arena, Model& model, StringView obj_data) {

    ModelCount count = model_count(obj_data);

    DynArray<Vector3> geometric_vertices { arena, count.n_verts };
    DynArray<Vector2> texture_vertices { arena, count.n_texts };
    DynArray<Vector3> vertex_normals { arena, count.n_norms };
    DynArray<FaceCorner> face_corners { arena, count.n_faces };

    while (obj_data.size()) {

        Cut cut_line = cut(obj_data, '\n');
        if (!cut_line.success) {
            return Error::ObjInvalidFormat;
        }

        obj_data = cut_line.right;

        StringView line = trim(cut_line.left);
        if (line.empty()) {
            continue;
        }

        {
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
    }

    model.geometric_vertices = geometric_vertices;
    model.texture_vertices = texture_vertices;
    model.vertex_normals = vertex_normals;
    model.face_corners = face_corners;

    return Error::Ok;
}

} // namespace inl
