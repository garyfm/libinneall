#include <libinneall/asset/obj.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/string.hpp>

#include <format>

namespace {

inl::obj::Result<float> extract_float(std::string_view buffer) {

    float value { 0 };

    std::string_view trimed = inl::trim(buffer);
    std::from_chars_result result = std::from_chars(trimed.data(), trimed.data() + trimed.size(), value);
    if (result.ec != std::errc()) {
        return std::unexpected(inl::obj::Error::FailedToExtractFloat);
    }
    return value;
}

unsigned map_index(inl::obj::Model const& model, float index) {
    if (index < 0) {
        // handle negative index
        return static_cast<unsigned>(model.geometric_vertices.size() + static_cast<unsigned>(index) + 1);
    } else {
        return static_cast<unsigned>(index);
    }
}

constexpr std::string_view DATA_TYPE_GEOMETRIC_VERTICES = "v";
constexpr std::string_view DATA_TYPE_TEXTURE_VERTICES = "vt";
constexpr std::string_view DATA_TYPE_VERTEX_NORMALS = "vn";
constexpr std::string_view DATA_TYPE_FACE = "f";

inl::obj::Result<inl::Vector3> parse_vector3(std::string_view data_values) {
    inl::Vector3 vector {};
    inl::Cut cut_vertex = inl::cut(data_values, ' ');

    inl::obj::Result<float> x = TRY(extract_float(cut_vertex.left));
    vector.x = *x;

    cut_vertex = inl::cut(inl::trim(cut_vertex.right), ' ');
    inl::obj::Result<float> y = TRY(extract_float(cut_vertex.left));
    vector.y = *y;

    inl::obj::Result<float> z = TRY(extract_float(cut_vertex.right));
    vector.z = *z;
    return vector;
}

}

namespace inl::obj {

Result<Model> load(std::string_view buffer) {
    Model model {};

    while (buffer.size()) {

        Cut cut_line = cut(buffer, '\n');
        if (!cut_line.success) {
            return std::unexpected(Error::InvalidFormat);
        }

        buffer = cut_line.right;

        std::string_view line = trim(cut_line.left);
        if (line.empty()) {
            continue;
        }

        {
            Cut cut_data = cut(line, ' ');

            std::string_view data_type = trim(cut_data.left);
            std::string_view data_values = trim(cut_data.right);

            if (data_type == DATA_TYPE_GEOMETRIC_VERTICES) {
                Result<Vector3> vector = TRY(parse_vector3(data_values));
                model.geometric_vertices.emplace_back(*vector);
            } else if (data_type == DATA_TYPE_TEXTURE_VERTICES) {
                Vector2 vertex_texture {};
                Cut cut_vertex_texture = cut(data_values, ' ');

                Result<float> x = TRY(extract_float(cut_vertex_texture.left));
                vertex_texture.x = *x;

                Result<float> y = TRY(extract_float(cut_vertex_texture.right));
                vertex_texture.y = *y;

                model.texture_vertices.emplace_back(vertex_texture);
            } else if (data_type == DATA_TYPE_VERTEX_NORMALS) {
                Result<Vector3> vector = TRY(parse_vector3(data_values));
                model.vertex_normals.emplace_back(*vector);
            } else if (data_type == DATA_TYPE_FACE) {
                Cut cut_face { .left = {}, .right = data_values, .success = true };
                while (cut_face.right.size() > 0) {
                    cut_face = cut(trim(cut_face.right), ' ');
                    Cut cut_index = cut(cut_face.left, '/');

                    FaceCorner corner {};
                    Result<float> vertex_index = TRY(extract_float(cut_index.left));
                    corner.vertex_index = map_index(model, *vertex_index);
                    INL_ASSERT(corner.vertex_index - 1 < model.geometric_vertices.size(),
                        std::format("Invalid obj vertex index: {}", corner.vertex_index));

                    // Cut successful, process next index
                    if (cut_index.success) {
                        cut_index = cut(cut_index.right, '/');

                        Result<float> texture_index = TRY(extract_float(cut_index.left));
                        corner.texture_index = map_index(model, *texture_index);
                        INL_ASSERT(corner.texture_index - 1 < model.texture_vertices.size(),
                            std::format("Invalid obj texture index: {}", corner.texture_index));

                        // Cut successfull, process next index
                        if (cut_index.success) {
                            Result<float> normal_index = TRY(extract_float(cut_index.right));
                            corner.normal_index = map_index(model, *normal_index);
                            INL_ASSERT(corner.normal_index - 1 < model.vertex_normals.size(),
                                std::format("Invalid obj normal index: {}", corner.normal_index));
                        }
                    }

                    model.face_corners.emplace_back(corner);
                }
            }
        }
    }

    return model;
}

} // namespace inl
