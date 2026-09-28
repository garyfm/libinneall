#include <libinneall/asset/obj.hpp>
#include <libinneall/base/array_list.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/hash_map.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/mesh_data.hpp>
#include <libinneall/vertex_data.hpp>

namespace inl {

MeshData to_mesh_data(Arena& arena, obj::Mesh const& model) {

    ArrayList<VertexData> vertex_data { arena, model.face_corners.size() };
    ArrayList<uint32_t> index_data { arena, model.face_corners.size() };

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
            // TOOD: no_grow
            vertex_map.insert(vertex, ebo_index);
            vertex_data.push(vertex);
            index_data.push(ebo_index);
            ++ebo_index;
        }
    }

    return { vertex_data, index_data };
}

void calculate_tangents(MeshData& mesh) {

    for (size_t i = 0; i < mesh.index_data.size(); i += 3) {
        uint32_t index = mesh.index_data[i];
        const Vector3 pos_1 = mesh.vertex_data[index].pos;
        const Vector2 uv_1 = mesh.vertex_data[index].uv;

        index = mesh.index_data[i + 1];
        const Vector3 pos_2 = mesh.vertex_data[index].pos;
        const Vector2 uv_2 = mesh.vertex_data[index].uv;

        index = mesh.index_data[i + 2];
        const Vector3 pos_3 = mesh.vertex_data[index].pos;
        const Vector2 uv_3 = mesh.vertex_data[index].uv;

        const Vector3 edge_1 = pos_2 - pos_1;
        const Vector3 edge_2 = pos_3 - pos_1;

        const Vector2 uv_delta_1 = uv_2 - uv_1;
        const Vector2 uv_delta_2 = uv_3 - uv_1;

        float f = 1.0f / (uv_delta_1.x * uv_delta_2.y - uv_delta_2.x * uv_delta_1.y);

        Vector3 tangent {};
        tangent.x = f * (uv_delta_2.y * edge_1.x - uv_delta_1.y * edge_2.x);
        tangent.y = f * (uv_delta_2.y * edge_1.y - uv_delta_1.y * edge_2.y);
        tangent.z = f * (uv_delta_2.y * edge_1.z - uv_delta_1.y * edge_2.z);

        Vector3 bitangent {};
        bitangent.x = f * (-uv_delta_2.x * edge_1.x + uv_delta_1.x * edge_2.x);
        bitangent.y = f * (-uv_delta_2.x * edge_1.y + uv_delta_1.x * edge_2.y);
        bitangent.z = f * (-uv_delta_2.x * edge_1.z + uv_delta_1.x * edge_2.z);

        index = mesh.index_data[i];
        mesh.vertex_data[index].tangent = tangent;
        mesh.vertex_data[index].bitangent = bitangent;

        index = mesh.index_data[i + 1];
        mesh.vertex_data[index].tangent = tangent;
        mesh.vertex_data[index].bitangent = bitangent;

        index = mesh.index_data[i + 2];
        mesh.vertex_data[index].tangent = tangent;
        mesh.vertex_data[index].bitangent = bitangent;
    }
}

} // namespace
