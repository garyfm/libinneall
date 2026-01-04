#include <libinneall/renderer/gl_buffer.hpp>

#include <span>

namespace inl {

GlBuffer::GlBuffer(std::span<std::byte const> data)
    : m_size { data.size() } {

    // Allocate a buffer handle
    glCreateBuffers(1, &m_handle);

    // Allocate a buffer storage
    glNamedBufferStorage(m_handle, data.size(), data.data(), GL_DYNAMIC_STORAGE_BIT);
}

void GlBuffer::delete_buffer(GLuint buffer) { glDeleteBuffers(1, &buffer); }

} // namespace inl
