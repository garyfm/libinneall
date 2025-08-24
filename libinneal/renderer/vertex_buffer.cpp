#include <libinneal/renderer/vertex_buffer.hpp>

#include <span>

namespace inl {

VertexBuffer::VertexBuffer(std::span<std::byte const> data)
    : m_data { data } {
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, data.size(), data.data(), GL_DYNAMIC_STORAGE_BIT);
}

void VertexBuffer::delete_buffer(GLuint buffer) { glDeleteBuffers(1, &buffer); }

} // namespace inl
