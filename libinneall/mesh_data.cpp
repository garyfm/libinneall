#include <libinneall/asset/obj.hpp>
#include <libinneall/mesh_data.hpp>

namespace inl {

MeshData to_mesh_data(obj::Model const& model) {

    MeshData mesh_data {};
    for (std::size_t i { 0 }; i < model.vertices.size(); ++i) {

        // TODO: Handle models without texture/normals in a cleaner way
        if (model.vertices_texture.empty()) {
            mesh_data.vertex_data.emplace_back(VertexData { model.vertices[i], {} });
        } else {
            mesh_data.vertex_data.emplace_back(VertexData { model.vertices[i], model.vertices_texture[i] });
        }
    }

    for (std::size_t i { 0 }; i < model.indices.size(); ++i) {
        // OBJ is 1 indices are 1-based
        mesh_data.index_data.emplace_back(model.indices[i].vertex_index - 1);
    }

    return mesh_data;
}

} // namespace
