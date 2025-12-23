#include <libinneall/asset/obj.hpp>
#include <libinneall/mesh_data.hpp>

namespace inl {

MeshData to_mesh_data(obj::Model const& model) {

    MeshData mesh_data {};
    for (std::size_t i { 0 }; i < model.geometric_vertices.size(); ++i) {
        if (model.texture_vertices.empty()) {
            mesh_data.vertex_data.emplace_back(VertexData { model.geometric_vertices[i], {} });
        } else {
            mesh_data.vertex_data.emplace_back(VertexData { model.geometric_vertices[i], model.texture_vertices[i] });
        }
    }

    for (std::size_t i { 0 }; i < model.faces.size(); ++i) {
        // OBJ is 1 faces are 1-based
        mesh_data.index_data.emplace_back(model.faces[i].vertex_index - 1);
    }

    return mesh_data;
}

} // namespace
