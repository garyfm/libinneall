#include <libinneall/asset/obj.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/mesh_data.hpp>
#include <libinneall/vertex_data.hpp>

#include <unordered_map>

namespace inl {

struct HasherVertexData {
    std::size_t operator()(const VertexData& key) const {
        using std::hash;

        std::size_t hash_pos
            = hash<float>()(key.pos.x) ^ (hash<float>()(key.pos.y) << 1) ^ (hash<float>()(key.pos.z) << 2);

        std::size_t hash_uv = hash<float>()(key.uv.x) ^ (hash<float>()(key.uv.y) << 1);
        std::size_t hash_normal
            = hash<float>()(key.normal.x) ^ (hash<float>()(key.normal.y) << 1) ^ (hash<float>()(key.normal.z) << 2);

        return hash_pos ^ (hash_uv << 1) ^ (hash_normal << 2);
    }
};

MeshData to_mesh_data(obj::Model const& model) {

    MeshData mesh_data {};
    std::unordered_map<VertexData, std::uint32_t, HasherVertexData> vertex_map;
    std::uint32_t ebo_index { 0 };

    INL_ASSERT(model.face_corners.size() % 3 == 0, "Mesh data is not triangulated");

    for (std::size_t i { 0 }; i < model.face_corners.size(); ++i) {

        obj::FaceCorner corner { model.face_corners[i] };
        // OBJ indices are 1 based
        std::size_t vertex_index = corner.vertex_index - 1;
        std::size_t texture_index = corner.texture_index - 1;
        std::size_t normal_index = corner.normal_index - 1;

        VertexData vertex {};
        vertex.pos = model.geometric_vertices[vertex_index];
        if (model.texture_vertices.size() != 0) {
            vertex.uv = model.texture_vertices[texture_index];
        }

        if (model.vertex_normals.size() != 0) {
            vertex.normal = model.vertex_normals[normal_index];
        }

        // Reuse ebo index for duplicate vertices
        if (auto found = vertex_map.find(vertex); found != vertex_map.end()) {
            // Vertex exsists already, reuse the ebo_index
            mesh_data.index_data.emplace_back(found->second);
        } else {
            // New vertex, insert it and increment ebo_index
            vertex_map.emplace(vertex, ebo_index);
            mesh_data.vertex_data.emplace_back(vertex);
            mesh_data.index_data.emplace_back(ebo_index);
            ++ebo_index;
        }
    }

    return mesh_data;
}

} // namespace
