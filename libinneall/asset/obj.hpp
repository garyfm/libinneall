#pragma once

#include <libinneall/math/vector2.hpp>
#include <libinneall/math/vector3.hpp>

#include <expected>
#include <string_view>
#include <vector>

namespace inl::obj {

struct FaceCorner {
    unsigned vertex_index;
    unsigned texture_index;
    unsigned normal_index;
};

struct Model {
    std::vector<Vector3> geometric_vertices;
    std::vector<Vector2> texture_vertices;
    std::vector<Vector3> vertex_normals;
    std::vector<FaceCorner> face_corners;
};

// TODO: Add structured error so the failed line can be reported
enum class Error {
    InvalidFormat,
    UnsupportedFormat,
    EOFReachedUnexpectedly,
    FailedToExtractInteger,
    FailedToExtractFloat,
    FaceNotTriangulated,
};

template <typename T> using Result = std::expected<T, Error>;
Result<Model> load(std::string_view raw_data);

} // namespace inl
