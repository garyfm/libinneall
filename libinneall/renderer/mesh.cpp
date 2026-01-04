#include <libinneall/renderer/mesh.hpp>
#include <span>

namespace inl {

Mesh::Mesh(MeshData const& mesh_data)
    : m_vertex_count { mesh_data.vertex_data.size() }
    , m_index_count { mesh_data.index_data.size() }
    , m_vertex_buffer { as_bytes(mesh_data.vertex_data.data(), mesh_data.vertex_data.size()) } {

    m_vertex_array.bind_vertex_buffer({
        .index = 0,
        .buffer = m_vertex_buffer,
        .offset_bytes = 0,
        .stride_bytes = sizeof(VertexData),
    });

    if (mesh_data.index_data.size() != 0) {
        m_index_buffer = GlBuffer(
            std::as_bytes(std::span<std::uint32_t const> { mesh_data.index_data.data(), mesh_data.index_data.size() }));
        m_vertex_array.bind_element_buffer(m_index_buffer);
    }

    m_vertex_array.set_attribute({
        .index = 0,
        .binding_index = 0,
        .n_components = sizeof(Vector3) / sizeof(float),
        .offset_bytes = offsetof(VertexData, pos),
        .type = GL_FLOAT,
        .normalise = false,
    });

    m_vertex_array.set_attribute({
        .index = 1,
        .binding_index = 0,
        .n_components = sizeof(Vector2) / sizeof(float),
        .offset_bytes = offsetof(VertexData, uv),
        .type = GL_FLOAT,
        .normalise = false,
    });

    m_vertex_array.set_attribute({
        .index = 2,
        .binding_index = 0,
        .n_components = sizeof(Vector3) / sizeof(float),
        .offset_bytes = offsetof(VertexData, normal),
        .type = GL_FLOAT,
        .normalise = false,
    });
}

void Mesh::bind() const { m_vertex_array.bind(); }
void Mesh::unbind() const { m_vertex_array.unbind(); }

} // namepsace inl
