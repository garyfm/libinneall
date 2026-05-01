#include <libinneall/renderer/gl_buffer.hpp>

namespace inl {

void GlBuffer::create(size_t size) {
    m_size = size;

    create_buffer(nullptr, size);
}

void GlBuffer::create(Span<uint8_t const> data) {
    m_size = data.size();

    create_buffer(data.data(), data.size());
}

void GlBuffer::allocate(Span<uint8_t const> data) {
    m_handle.reset();
    m_size = 0;

    create_buffer(data.data(), data.size());

    m_size = data.size();
}

void GlBuffer::allocate(size_t size) {
    m_handle.reset();
    m_size = 0;

    create_buffer(nullptr, size);

    m_size = size;
}

void GlBuffer::upload(size_t offset, Span<uint8_t const> data) {
    INL_ASSERT(m_handle != 0, "Accessing invalid handle");
    glNamedBufferSubData(m_handle, offset, data.size(), data.data());
}

void GlBuffer::create_buffer(uint8_t const* data, size_t size) {
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, size, data, GL_DYNAMIC_STORAGE_BIT);
}

void GlBuffer::delete_buffer(GLuint buffer) { glDeleteBuffers(1, &buffer); }

} // namespace inl
