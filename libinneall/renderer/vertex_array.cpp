#include <libinneall/renderer/vertex_array.hpp>

namespace inl {

VertexArray::VertexArray() { glCreateVertexArrays(1, &m_handle); }

void VertexArray::delete_array(GLuint buffer) { glDeleteVertexArrays(1, &buffer); }

void VertexArray::bind_vertex_buffer(BindPoint const& bind_point) const {

    // Bind the vertex buffer to a bind point
    glVertexArrayVertexBuffer(m_handle, static_cast<GLuint>(bind_point.index), bind_point.buffer.native_handle(),
        bind_point.offset_bytes, static_cast<GLsizei>(bind_point.stride_bytes));
}

void VertexArray::bind_element_buffer(GlBuffer& buffer) const {

    glVertexArrayElementBuffer(m_handle, buffer.native_handle());
}

void VertexArray::set_attribute(Attribute const& attribute) const {

    // Enable the attribtute
    glEnableVertexArrayAttrib(m_handle, static_cast<GLuint>(attribute.index));

    // Attribute settings
    glVertexArrayAttribFormat(m_handle, static_cast<GLuint>(attribute.index),
        static_cast<GLint>(attribute.n_components), attribute.type, attribute.normalise,
        static_cast<GLsizei>(attribute.stride_bytes));

    // Bind the attribute to a bind point
    glVertexArrayAttribBinding(
        m_handle, static_cast<GLuint>(attribute.index), static_cast<GLuint>(attribute.binding_index));
}

} // namespace inl
