#include <libinneall/asset/obj.hpp>
#include <libinneall/base/array_dyn.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/hash_map.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/mesh_data.hpp>
#include <libinneall/vertex_data.hpp>

namespace inl {

MeshData to_mesh_data(Arena& arena, obj::Model const& model) {

    ArrayDyn<VertexData> vertex_data { arena, model.face_corners.size() };
    ArrayDyn<uint32_t> index_data { arena, model.face_corners.size() };

    auto vertex_map = HashMap<VertexData, uint32_t>::create(arena, model.face_corners.size());
    uint32_t ebo_index { 0 };

    inl_assert(model.face_corners.size() % 3 == 0, "Mesh data is not triangulated");

    for (size_t i { 0 }; i < model.face_corners.size(); ++i) {

        obj::FaceCorner corner { model.face_corners[i] };
        // OBJ indices are 1 based
        size_t vertex_index = corner.vertex_index - 1;
        size_t texture_index = corner.texture_index - 1;
        size_t normal_index = corner.normal_index - 1;

        VertexData vertex {};
        vertex.pos = model.geometric_vertices[vertex_index];
        if (model.texture_vertices.size() != 0) {
            vertex.uv = model.texture_vertices[texture_index];
        }

        if (model.vertex_normals.size() != 0) {
            vertex.normal = model.vertex_normals[normal_index];
        }

        // Reuse ebo index for duplicate vertices
        if (auto found = vertex_map.find(vertex); found.has_value()) {
            // Vertex exsists already, reuse the ebo_index
            index_data.push(*found.value());
        } else {
            // New vertex, insert it and increment ebo_index
            vertex_map.insert(vertex, ebo_index);
            vertex_data.push(vertex);
            index_data.push(ebo_index);
            ++ebo_index;
        }
    }

    return { vertex_data, index_data };
}

} // namespace
