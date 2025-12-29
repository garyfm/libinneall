#pragma once

#include <libinneall/asset/obj.hpp>
#include <libinneall/vertex_data.hpp>

namespace inl {

struct MeshData {
    std::vector<VertexData> vertex_data;
    std::vector<std::uint32_t> index_data;
};

MeshData to_mesh_data(obj::Model const& model);

} // namespace inl
