#include <libinneall/asset/obj.hpp>
#include <libinneall/mesh_data.hpp>

namespace inl {

MeshData to_mesh_data(obj::Model const& model) {

    MeshData mesh_data {};
    for (std::size_t i { 0 }; i < model.vertices.size(); ++i) {
        mesh_data.vertex_data.emplace_back(model.vertices[i]);
    }

    for (std::size_t i { 0 }; i < model.indices.size(); ++i) {
        mesh_data.index_data.emplace_back(model.indices[i]);
    }

    return mesh_data;
}

} // namespace
