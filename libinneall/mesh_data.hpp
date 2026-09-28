#pragma once

#include <libinneall/asset/obj.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/vertex_data.hpp>

namespace inl {

struct MeshData {
    Span<VertexData> vertex_data;
    Span<uint32_t> index_data;
};

MeshData to_mesh_data(Arena& arena, obj::Mesh const& model);

void calculate_tangents(MeshData& mesh);

} // namespace inl
