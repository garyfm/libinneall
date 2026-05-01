#pragma once

#include <libinneall/base/unique_handle.hpp>
#include <libinneall/renderer/gl_buffer.hpp>
#include <subprojects/glad/include/glad/glad.h>

namespace inl {

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

    explicit VertexArray();

    GLuint handle() const {
        INL_ASSERT(m_handle != 0, "Accessing invalid handle");
        return m_handle;
    }

    void set_attribute(Attribute const& attribute) const;

    void bind_vertex_buffer(BindPoint const& bind_point) const;
    void bind_element_buffer(GlBuffer& buffer) const;

    void bind() const { glBindVertexArray(m_handle); }
    void unbind() const { glBindVertexArray(0); }

private:
    static void delete_array(GLuint array);

    UniqueHandle<GLuint, delete_array> m_handle { 0 };
};

} // namespace inl
