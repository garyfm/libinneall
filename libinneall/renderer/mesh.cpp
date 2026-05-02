#include <libinneall/base/byte.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/renderer/mesh.hpp>

namespace inl {

Error Mesh::create(Mesh& mesh, MeshData const& mesh_data) {
    mesh.m_vertex_count = mesh_data.vertex_data.size();
    mesh.m_index_count = mesh_data.index_data.size();

    Error error = GlBuffer::create(
        mesh.m_vertex_buffer, to_bytes(Span { mesh_data.vertex_data.data(), mesh_data.vertex_data.size() }));
    if (error != Error::Ok) {
        return error;
    }

    mesh.m_vertex_array.bind_vertex_buffer({
        .index = 0,
        .buffer = mesh.m_vertex_buffer,
        .offset_bytes = 0,
        .stride_bytes = sizeof(VertexData),
    });

    if (mesh_data.index_data.size() != 0) {
        error = GlBuffer::create(
            mesh.m_index_buffer, to_bytes(Span { mesh_data.index_data.data(), mesh_data.index_data.size() }));
        if (error != Error::Ok) {
            return error;
        }

        mesh.m_vertex_array.bind_element_buffer(mesh.m_index_buffer);
    }

    mesh.m_vertex_array.set_attribute({
        .index = 0,
        .binding_index = 0,
        .n_components = sizeof(Vector3) / sizeof(float),
        .offset_bytes = offsetof(VertexData, pos),
        .type = GL_FLOAT,
        .normalise = false,
    });

    mesh.m_vertex_array.set_attribute({
        .index = 1,
        .binding_index = 0,
        .n_components = sizeof(Vector2) / sizeof(float),
        .offset_bytes = offsetof(VertexData, uv),
        .type = GL_FLOAT,
        .normalise = false,
    });

    mesh.m_vertex_array.set_attribute({
        .index = 2,
        .binding_index = 0,
        .n_components = sizeof(Vector3) / sizeof(float),
        .offset_bytes = offsetof(VertexData, normal),
        .type = GL_FLOAT,
        .normalise = false,
    });

    mesh.m_created = true;

    return Error::Ok;
}

Mesh::~Mesh() {
    if (is_bound()) {
        unbind();
    }
}

void Mesh::bind() {
    INL_ASSERT(m_created, "Invalid Mesh");

    m_vertex_array.bind();
    m_is_bound = true;
}

void Mesh::unbind() {
    INL_ASSERT(m_created, "Invalid Mesh");
    m_vertex_array.unbind();
    m_is_bound = false;
}

} // namepsace inl
