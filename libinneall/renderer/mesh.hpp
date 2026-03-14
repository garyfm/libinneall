#pragma once

#include <libinneall/mesh_data.hpp>
#include <libinneall/renderer/gl_buffer.hpp>
#include <libinneall/renderer/vertex_array.hpp>

#include <vector>

namespace inl {

class Mesh {
public:
    explicit Mesh(MeshData const& data);

    Mesh(Mesh const&) = delete;
    Mesh operator=(Mesh const&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    ~Mesh();

    void bind();
    void unbind();
    bool is_bound() const { return m_is_bound; }

    size_t vertext_count() const { return m_vertex_count; };
    size_t index_count() const { return m_index_count; };

private:
    size_t m_vertex_count {};
    size_t m_index_count {};

    GlBuffer m_vertex_buffer {};
    GlBuffer m_index_buffer {};
    VertexArray m_vertex_array {};

    bool m_is_bound { false };
};

} // namespace inl
