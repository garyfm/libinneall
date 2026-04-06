#pragma once

#include <libinneall/base/string_view.hpp>
#include <libinneall/math/vector2.hpp>
#include <libinneall/math/vector3.hpp>

#include <expected>
#include <vector>

namespace inl::obj {

struct FaceCorner {
    uint32_t vertex_index;
    uint32_t texture_index;
    uint32_t normal_index;
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
Result<Model> load(StringView raw_data);

} // namespace inl
