#pragma once

#include <libinneall/base/arena.hpp>
#include <libinneall/base/error.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/base/string_view.hpp>
#include <libinneall/math/vector2.hpp>
#include <libinneall/math/vector3.hpp>

namespace inl::obj {

struct FaceCorner {
    uint32_t vertex_index;
    uint32_t texture_index;
    uint32_t normal_index;
};

struct Model {
    Span<Vector3> geometric_vertices;
    Span<Vector2> texture_vertices;
    Span<Vector3> vertex_normals;
    Span<FaceCorner> face_corners;
};

Error load(Model& model, StringView raw_data);
Error load(Arena& arena, Model& model, StringView raw_data);

} // namespace inl
