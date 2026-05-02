#include <libinneall/renderer/vertex_array.hpp>

namespace inl {

Error VertexArray::create(VertexArray& vertex_array) {

    vertex_array.m_handle.reset();
    glCreateVertexArrays(1, &vertex_array.m_handle);

    if (vertex_array.m_handle == 0) {
        return Error::RendererVertexArrayFailedToCreate;
    }
    return Error::Ok;
}

void VertexArray::bind_vertex_buffer(BindPoint const& bind_point) const {
    INL_ASSERT(m_handle, "Invalid VertexArray");

    // Bind the vertex buffer to a bind point
    glVertexArrayVertexBuffer(m_handle, static_cast<GLuint>(bind_point.index), bind_point.buffer.handle(),
        bind_point.offset_bytes, static_cast<GLsizei>(bind_point.stride_bytes));
}

void VertexArray::bind_element_buffer(GlBuffer& buffer) const {
    INL_ASSERT(m_handle, "Invalid VertexArray");
    glVertexArrayElementBuffer(m_handle, buffer.handle());
}

void VertexArray::set_attribute(Attribute const& attribute) const {
    INL_ASSERT(m_handle, "Invalid VertexArray");

    // Enable the attribtute
    glEnableVertexArrayAttrib(m_handle, static_cast<GLuint>(attribute.index));

    // Attribute settings
    glVertexArrayAttribFormat(m_handle, static_cast<GLuint>(attribute.index),
        static_cast<GLint>(attribute.n_components), attribute.type, attribute.normalise,
        static_cast<GLsizei>(attribute.offset_bytes));

    // Bind the attribute to a bind point
    glVertexArrayAttribBinding(
        m_handle, static_cast<GLuint>(attribute.index), static_cast<GLuint>(attribute.binding_index));
}

} // namespace inl
