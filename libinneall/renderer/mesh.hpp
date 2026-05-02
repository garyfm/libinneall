#pragma once

#include <libinneall/base/utility.hpp>
#include <libinneall/mesh_data.hpp>
#include <libinneall/renderer/gl_buffer.hpp>
#include <libinneall/renderer/vertex_array.hpp>

namespace inl {

class Mesh {
public:
    Mesh() = default;
    static Error create(Mesh& mesh, MeshData const& data);

    INL_DEL_COPY_MOVE(Mesh);

    ~Mesh();

    void bind();
    void unbind();
    bool is_bound() const { return m_is_bound; }

    size_t vertext_count() const { return m_vertex_count; };
    size_t index_count() const { return m_index_count; };

private:
    bool m_created { false };

    size_t m_vertex_count {};
    size_t m_index_count {};

    GlBuffer m_vertex_buffer {};
    GlBuffer m_index_buffer {};
    VertexArray m_vertex_array {};

    bool m_is_bound { false };
};

} // namespace inl
