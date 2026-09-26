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

struct Mtl {
    StringView name;
    StringView map_albedo;
    StringView map_normal;
    StringView map_specular;

    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    float shininess;
    float dissolve;
};

struct Mesh {
    Span<Vector3> geometric_vertices;
    Span<Vector2> texture_vertices;
    Span<Vector3> vertex_normals;
    Span<FaceCorner> face_corners;
};

Error load(Arena& arena, Mesh& mesh, StringView obj_data);
Error load(Mtl& mtl, StringView mtl_data);

} // namespace inl
