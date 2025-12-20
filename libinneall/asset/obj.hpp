
#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <libinneall/math/vector3.hpp>
#include <libinneall/renderer/vertex_data.hpp>
#include <string_view>
#include <vector>

namespace inl::obj {

struct Model {
    std::vector<VertexData> vertices;
    std::vector<unsigned> indices;
};

enum class Error {
    InvalidFormat,
    UnsupportedFormat,
    EOFReachedUnexpectedly,
    FailedToExtractInteger,
    FaceNotTriangulated,
};

template <typename T> using Result = std::expected<T, Error>;
Result<Model> load(std::string_view raw_data);

} // namespace inl
