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

Mesh::Mesh(Mesh&& other) noexcept
    : m_vertex_count { other.m_vertex_count }
    , m_index_count { other.m_index_count }
    , m_vertex_buffer { std::move(other.m_vertex_buffer) }
    , m_index_buffer { std::move(other.m_index_buffer) }
    , m_vertex_array { std::move(other.m_vertex_array) } {
    other.m_vertex_count = 0;
    other.m_index_count = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {

    if (this != std::addressof(other)) {
        if (is_bound()) {
            unbind();
        }
        m_vertex_count = other.m_vertex_count;
        m_index_count = other.m_index_count;
        m_vertex_buffer = std::move(other.m_vertex_buffer);
        m_index_buffer = std::move(other.m_index_buffer);
        m_vertex_array = std::move(other.m_vertex_array);

        other.m_vertex_count = 0;
        other.m_index_count = 0;
    }

    return *this;
}

Mesh::~Mesh() {
    if (is_bound()) {
        unbind();
    }
}

void Mesh::bind() {
    m_vertex_array.bind();
    m_is_bound = true;
}

void Mesh::unbind() {
    m_vertex_array.unbind();
    m_is_bound = false;
}

} // namepsace inl
