#pragma once

#include <libinneall/math/vector2.hpp>
#include <libinneall/math/vector3.hpp>

namespace inl {

struct VertexData {
    Vector3 pos;
    Vector2 uv;
    Vector3 normal;

    bool operator==(const VertexData& other) const {
        return (pos == other.pos && uv == other.uv) && (normal == other.normal);
    }
};

} // namespace inl
