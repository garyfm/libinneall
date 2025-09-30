#pragma once

#include <libinneall/renderer/gl_buffer.hpp>
#include <libinneall/renderer/vertex_array.hpp>
#include <libinneall/renderer/vertex_data.hpp>

#include <span>

namespace inl {

struct MeshData {
    std::span<VertexData const> vertex_data;
    std::span<unsigned const> index_data;
};

class Mesh {
public:
    explicit Mesh(MeshData const& data);

    void bind() const;

    std::size_t vertext_count() const { return m_data.vertex_data.size(); };
    std::size_t index_count() const { return m_data.index_data.size(); };

private:
    MeshData m_data {};
    GlBuffer m_vertex_buffer {};
    GlBuffer m_index_buffer {};
    VertexArray m_vertex_array {};
};

} // namespace inl
