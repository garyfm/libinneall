#pragma once

#include <libinneall/math/vector2.hpp>
#include <libinneall/math/vector3.hpp>

#include <span>

namespace inl {

struct VertexData {
    Vector3 pos;
    Vector2 uv;
    Vector3 normal;

    bool operator==(const VertexData& other) const {
        return (pos == other.pos && uv == other.uv) && (normal == other.normal);
    }
};

std::span<const std::byte> as_bytes(VertexData const* const vertex_data, std::size_t count);

} // namespace inl
