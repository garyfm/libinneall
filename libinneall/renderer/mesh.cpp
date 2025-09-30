#include <libinneall/renderer/mesh.hpp>
#include <span>

namespace inl {

Mesh::Mesh(MeshData const& data)
    : m_data { data }
    , m_vertex_buffer { std::as_bytes(m_data.vertex_data) } {

    m_vertex_array.bind_vertex_buffer({
        .index = 0,
        .buffer = m_vertex_buffer,
        .offset_bytes = 0,
        .stride_bytes = sizeof(VertexData),
    });

    if (m_data.index_data.size() != 0) {
        m_index_buffer = GlBuffer(std::as_bytes(m_data.index_data));
        m_vertex_array.bind_element_buffer(m_index_buffer);
    }

    m_vertex_array.set_attribute({
        .index = 0,
        .binding_index = 0,
        .n_components = sizeof(VertexData) / sizeof(float),
        .stride_bytes = 0,
        .type = GL_FLOAT,
        .normalise = false,
    });
}

void Mesh::bind() const { m_vertex_array.bind(); }

} // namepsace inl
