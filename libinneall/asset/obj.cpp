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

// check if model gets copied here?
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

            if (line[cursor] == 'v') {
                ++cursor;
                VertexData vertex {};
                std::size_t component_index { 0 };
                while (cursor < line.size()) {
                    // skip whitespace
                    if (std::isspace(line[cursor])) {
                        ++cursor;
                        continue;
                    }

                    Result<float> component = TRY(extract_integer(line, cursor));
                    log::debug("v component: {}", *component);
                    vertex.pos.elements[component_index++] = *component;
                }
                model.vertices.emplace_back(vertex);
            } else if (line[cursor] == 'f') {
                ++cursor;
                while (cursor < line.size()) {
                    // skip whitespace
                    if (std::isspace(line[cursor])) {
                        ++cursor;
                        continue;
                    }

                    Result<float> index = TRY(extract_integer(line, cursor));
                    log::debug("f component: {}", *index);

                    if (*index < 0) {
                        // handle negative index
                        model.indices.emplace_back((model.vertices.size()) + static_cast<unsigned>(*index));
                    } else {
                        // obj index's from 1 so this needs to convert to zero index
                        model.indices.emplace_back(static_cast<unsigned>(*index) - 1);
                    }
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
