#include <libinneall/asset/obj.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/string.hpp>

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

constexpr std::string_view DATA_TYPE_VERTEX = "v";
constexpr std::string_view DATA_TYPE_VERTEX_TEXTURE = "vt";
constexpr std::string_view DATA_TYPE_FACE = "f";

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

        log::debug("line: {}", line);

        {
            Cut cut_data = cut(line, ' ');

            std::string_view data_type = trim(cut_data.left);
            std::string_view data_values = trim(cut_data.right);

            log::debug("data_type: {}", data_type);
            log::debug("data_values: {}", data_values);
            if (data_type == DATA_TYPE_VERTEX) {
                Vector3 vertex {};
                Cut cut_vertex = cut(data_values, ' ');

                log::debug("cut_vertex left: {}", cut_vertex.left);
                log::debug("cut_vertex right: {}", cut_vertex.right);
                Result<float> x = TRY(extract_float(cut_vertex.left));
                log::debug("v component: {}", *x);
                vertex.x = *x;

                cut_vertex = cut(trim(cut_vertex.right), ' ');
                Result<float> y = TRY(extract_float(cut_vertex.left));
                log::debug("v component: {}", *y);
                vertex.y = *y;

                Result<float> z = TRY(extract_float(cut_vertex.right));
                log::debug("v component: {}", *z);
                vertex.z = *z;

                model.geometric_vertices.emplace_back(vertex);
            } else if (data_type == DATA_TYPE_VERTEX_TEXTURE) {
                Vector2 vertex_texture {};
                Cut cut_vertex_texture = cut(data_values, ' ');

                Result<float> x = TRY(extract_float(cut_vertex_texture.left));
                log::debug("vt component: {}", *x);
                vertex_texture.x = *x;

                Result<float> y = TRY(extract_float(cut_vertex_texture.right));
                log::debug("v component: {}", *y);
                vertex_texture.y = *y;

                model.texture_vertices.emplace_back(vertex_texture);
            } else if (data_type == DATA_TYPE_FACE) {
                Cut cut_face { .left = {}, .right = data_values, .success = true };
                while (cut_face.right.size() > 0) {
                    cut_face = cut(trim(cut_face.right), ' ');
                    Cut cut_index = cut(cut_face.left, '/');

                    Face face {};
                    Result<float> vertex_index = TRY(extract_float(cut_index.left));
                    face.vertex_index = map_index(model, *vertex_index);

                    if (cut_index.success) {
                        Result<float> texture_index = TRY(extract_float(cut_index.right));
                        face.texture_index = map_index(model, *texture_index);
                    }

                    log::debug("f component: {}/{}", face.vertex_index, face.texture_index);
                    model.faces.emplace_back(face);
                }
            }
        }
    }

    return model;
}

} // namespace inl
