#pragma once

#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_handle.hpp>
#include <libinneall/base/utility.hpp>
#include <libinneall/renderer/gl_buffer.hpp>
#include <subprojects/glad/include/glad/glad.h>

namespace inl {

inline void delete_vertex_array(GLuint handle) { glDeleteVertexArrays(1, &handle); }

class VertexArray {
public:
    struct Attribute {
        size_t index;
        size_t binding_index;
        size_t n_components;
        size_t offset_bytes;
        GLenum type;
        bool normalise;
    };

    struct BindPoint {
        size_t index;
        GlBuffer const& buffer;
        size_t offset_bytes;
        size_t stride_bytes;
    };

    VertexArray() = default;

    static Error create(VertexArray& vertex_array);

    INL_DEL_COPY_MOVE(VertexArray);

    GLuint handle() const { return m_handle; }

    void set_attribute(Attribute const& attribute) const;

    void bind_vertex_buffer(BindPoint const& bind_point) const;
    void bind_element_buffer(GlBuffer& buffer) const;

    void bind() const {
        INL_ASSERT(m_handle, "Invalid VertexArray");
        glBindVertexArray(m_handle);
    }

    void unbind() const {
        INL_ASSERT(m_handle, "Invalid VertexArray");
        glBindVertexArray(0);
    }

private:
    UniqueHandle<GLuint, delete_vertex_array> m_handle { 0 };
};

} // namespace inl
