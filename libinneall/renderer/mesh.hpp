#pragma once

#include <libinneall/mesh_data.hpp>
#include <libinneall/renderer/gl_buffer.hpp>
#include <libinneall/renderer/vertex_array.hpp>

#include <vector>

namespace inl {

class Mesh {
public:
    explicit Mesh(MeshData const& data);

    void bind() const;
    void unbind() const;

    std::size_t vertext_count() const { return m_vertex_count; };
    std::size_t index_count() const { return m_index_count; };

private:
    std::size_t m_vertex_count {};
    std::size_t m_index_count {};
    GlBuffer m_vertex_buffer {};
    GlBuffer m_index_buffer {};
    VertexArray m_vertex_array {};
};

} // namespace inl
