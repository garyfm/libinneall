#include <libinneall/asset/obj.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/result.hpp>

namespace {

inl::obj::Result<float> extract_integer(std::string_view line, std::size_t& cursor) {

    float value { 0 };
    std::from_chars_result result = std::from_chars(&line[cursor], line.data() + line.size(), value);
    if (result.ec != std::errc()) {
        return std::unexpected(inl::obj::Error::FailedToExtractInteger);
    }
    cursor = result.ptr - line.data();
    return value;
}

}
namespace inl::obj {

static constexpr std::string_view DATA_TYPE_VERTEX = "v ";
static constexpr std::string_view DATA_TYPE_VERTEX_TEXTURE = "vt";
static constexpr std::string_view DATA_TYPE_FACE = "f ";

Result<Model> load(std::string_view data) {
    Model model {};

    std::size_t line_start { 0 };

    while (line_start < data.size()) {

        // skip whitespace
        if (std::isspace(data[line_start])) {
            ++line_start;
            continue;
        }

        std::string_view::size_type line_end = data.find('\n', line_start);

        if (line_end == std::string_view::npos) {
            line_end = data.size();
        }

        std::string_view line = data.substr(line_start, line_end - line_start);
        log::debug("obj line: {}", line);
        line_start = line_end + 1;

        {
            std::size_t cursor { 0 };

            std::string_view data_type = line.substr(cursor, 2);

            if (data_type == DATA_TYPE_VERTEX) {
                ++cursor;
                Vector3 vertex {};
                std::size_t component_index { 0 };
                while (cursor < line.size()) {
                    // skip whitespace
                    if (std::isspace(line[cursor])) {
                        ++cursor;
                        continue;
                    }

                    Result<float> component = TRY(extract_integer(line, cursor));
                    log::debug("v component: {}", *component);
                    vertex.elements[component_index++] = *component;
                }
                model.vertices.emplace_back(vertex);
            } else if (data_type == DATA_TYPE_VERTEX_TEXTURE) {
                // TODO: Reduce code dumplication
                ++cursor;
                ++cursor;
                Vector2 uv {};
                std::size_t component_index { 0 };
                while (cursor < line.size()) {
                    // skip whitespace
                    if (std::isspace(line[cursor])) {
                        ++cursor;
                        continue;
                    }

                    Result<float> component = TRY(extract_integer(line, cursor));
                    log::debug("vt component: {}", *component);
                    uv.elements[component_index++] = *component;
                }
                model.vertices_texture.emplace_back(uv);
            } else if (data_type == DATA_TYPE_FACE) {
                ++cursor;
                while (cursor < line.size()) {
                    // skip whitespace
                    if (std::isspace(line[cursor])) {
                        ++cursor;
                        continue;
                    }

                    // TODO: Handle models without texture/normals in a cleaner way
                    // Parsing code is a mess
                    Result<float> vertex_index = TRY(extract_integer(line, cursor));
                    float texture_index { 0 };
                    if (line.contains('/')) {
                        // Skip '/'
                        ++cursor;
                        Result<float> result = TRY(extract_integer(line, cursor));
                        texture_index = *result;
                        log::debug("f component: {}/{}", *vertex_index, texture_index);
                    } else {
                        log::debug("f component: {}", *vertex_index);
                    }

                    auto map_index = [&model](float index) -> unsigned {
                        if (index < 0) {
                            // handle negative index
                            return static_cast<unsigned>(model.vertices.size() + static_cast<unsigned>(index) + 1);
                        } else {
                            return static_cast<unsigned>(index);
                        }
                    };

                    model.indices.emplace_back(Face { map_index(*vertex_index), map_index(texture_index) });
                }

                if (model.indices.size() % 3 != 0) {
                    return std::unexpected { Error::FaceNotTriangulated };
                }
            }
        }
    }

    return model;
}

} // namespace inl
