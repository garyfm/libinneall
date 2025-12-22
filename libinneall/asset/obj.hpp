#pragma once

#include <libinneall/math/vector2.hpp>
#include <libinneall/math/vector3.hpp>

#include <expected>
#include <string_view>
#include <vector>

namespace inl::obj {

struct Face {
    unsigned vertex_index;
    unsigned texture_index;
};

struct Model {
    std::vector<Vector3> vertices;
    std::vector<Vector2> vertices_texture;
    std::vector<Face> indices;
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
