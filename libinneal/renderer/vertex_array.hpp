#pragma once

#include <libinneal/glad/include/glad/glad.h>
#include <libinneal/renderer/vertex_buffer.hpp>
#include <libinneal/utility/unique_resource.hpp>

namespace inl {

class VertexArray {
public:
    explicit VertexArray();

    GLuint native_handle() const { return m_handle; }

    template <typename T>
    void set_attribute(std::size_t attribute_index, std::size_t binding_index, std::size_t attribute_size,
        bool normalise, std::size_t offset) const;

    template <typename T>
    void bind_vertex_buffer(
        std::size_t attribute_index, VertexBuffer const& vertex_buffer, std::size_t offset, std::size_t stride) const;

    void bind() const { glBindVertexArray(m_handle); }
    void unbind() const { glBindVertexArray(0); }

private:
    static void delete_array(GLuint array);

    UniqueResource<GLuint, decltype(&delete_array)> m_handle { 0, delete_array };
};

template <typename T>
void VertexArray::set_attribute(std::size_t attribute_index, std::size_t buffer_index, std::size_t attribute_size,
    bool normalise, std::size_t offset) const {

    GLenum type {};
    if constexpr (std::is_same_v<T, float>) {
        type = GL_FLOAT;
    } else {
        static_assert(std::is_same_v<T, float>, "Error: Attribute data of type T is not implemented");
    }

    glEnableVertexArrayAttrib(m_handle, attribute_index);

    glVertexArrayAttribFormat(m_handle, attribute_index, attribute_size, type, normalise, sizeof(T) * offset);

    glVertexArrayAttribBinding(m_handle, attribute_index, buffer_index);
}

template <typename T>
void VertexArray::bind_vertex_buffer(
    std::size_t binding_index, VertexBuffer const& vertex_buffer, std::size_t offset, std::size_t stride) const {
    glVertexArrayVertexBuffer(
        m_handle, binding_index, vertex_buffer.native_handle(), sizeof(T) * offset, sizeof(T) * stride);
}

}
