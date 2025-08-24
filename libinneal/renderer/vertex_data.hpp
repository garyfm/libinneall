#pragma once

#include <libinneal/math/vector3.hpp>

#include <span>

namespace inl {

struct VertexData {
    Vector3 pos;
};

std::span<const std::byte> as_bytes(VertexData const* const vertex_data, std::size_t count);

} // namespace inl
