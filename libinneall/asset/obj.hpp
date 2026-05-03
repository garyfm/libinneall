#pragma once

#include <libinneall/base/error.hpp>
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

// TODO:: Rework to use arena
struct Model {
    std::vector<Vector3> geometric_vertices;
    std::vector<Vector2> texture_vertices;
    std::vector<Vector3> vertex_normals;
    std::vector<FaceCorner> face_corners;
};

Error load(Model& model, StringView raw_data);

} // namespace inl
