#include <libinneall/renderer/gl_buffer.hpp>

namespace {

inl::Error create_buffer(GLuint* handle, uint8_t const* data, size_t size) {
    // NOTE: You might want to do this ? unsure
    inl_assert(size != 0, "Creating zero sized GlBuffer");

    glCreateBuffers(1, handle);
    if (handle == 0) {
        return inl::Error::RendererFailedToCreateGlBuffer;
    }

    glNamedBufferStorage(*handle, size, data, GL_DYNAMIC_STORAGE_BIT);

    return inl::Error::Ok;
}

}
namespace inl {

Error GlBuffer::create(GlBuffer& buffer, size_t size) {
    buffer.m_size = size;

    return create_buffer(buffer.m_handle.address_of(), nullptr, size);
}

Error GlBuffer::create(GlBuffer& buffer, Span<uint8_t const> data) {
    buffer.m_size = data.size();

    return create_buffer(buffer.m_handle.address_of(), data.data(), data.size());
}

void GlBuffer::upload(Span<uint8_t const> data, size_t offset) {
    inl_assert(m_handle != 0, "Invalid GlBuffer");
    inl_assert(data.size() <= m_size, "Data exceeds GlBuffer");

    glNamedBufferSubData(m_handle, offset, data.size(), data.data());
}

} // namespace inl
