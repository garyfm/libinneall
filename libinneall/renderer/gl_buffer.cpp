#include <libinneall/renderer/gl_buffer.hpp>

#include <span>

namespace inl {

GlBuffer::GlBuffer(std::size_t size)
    : m_size { size } {

    create_buffer(nullptr, size);
}

GlBuffer::GlBuffer(std::span<std::byte const> data)
    : m_size { data.size() } {

    create_buffer(data.data(), data.size());
}

void GlBuffer::allocate(std::span<std::byte const> data) {
    m_handle.reset();
    m_size = 0;

    create_buffer(data.data(), data.size());

    m_size = data.size();
}

void GlBuffer::allocate(std::size_t size) {
    m_handle.reset();
    m_size = 0;

    create_buffer(nullptr, size);

    m_size = size;
}

void GlBuffer::upload(std::size_t offset, std::span<std::byte const> data) {
    glNamedBufferSubData(m_handle, offset, data.size(), data.data());
}

void GlBuffer::create_buffer(std::byte const* data, std::size_t size) {
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, size, data, GL_DYNAMIC_STORAGE_BIT);
}

void GlBuffer::delete_buffer(GLuint buffer) { glDeleteBuffers(1, &buffer); }

} // namespace inl
