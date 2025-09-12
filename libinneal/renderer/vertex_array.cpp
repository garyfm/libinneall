#include <libinneal/renderer/vertex_array.hpp>

namespace inl {

VertexArray::VertexArray() { glCreateVertexArrays(1, &m_handle); }

void VertexArray::delete_array(GLuint buffer) { glDeleteVertexArrays(1, &buffer); }

void VertexArray::bind_vertex_buffer(BindPoint const& bind_point) const {

    // Bind the vertex buffer to a bind point
    glVertexArrayVertexBuffer(m_handle, bind_point.index, bind_point.buffer.native_handle(), bind_point.offset_bytes,
        bind_point.stride_bytes);
}

void VertexArray::bind_element_buffer(GlBuffer& buffer) const {

    glVertexArrayElementBuffer(m_handle, buffer.native_handle());
}

void VertexArray::set_attribute(Attribute const& attribute) const {

    // Enable the attribtute
    glEnableVertexArrayAttrib(m_handle, attribute.index);

    // Attribute settings
    glVertexArrayAttribFormat(
        m_handle, attribute.index, attribute.n_components, attribute.type, attribute.normalise, attribute.stride_bytes);

    // Bind the attribute to a bind point
    glVertexArrayAttribBinding(m_handle, attribute.index, attribute.binding_index);
}

} // namespace inl
