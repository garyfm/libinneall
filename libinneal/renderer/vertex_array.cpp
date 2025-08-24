#include <libinneal/renderer/vertex_array.hpp>

namespace inl {

VertexArray::VertexArray() { glCreateVertexArrays(1, &m_handle); }

void VertexArray::delete_array(GLuint buffer) { glDeleteVertexArrays(1, &buffer); }

} // namespace inl
