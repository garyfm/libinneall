#pragma once

#include <libinneall/base/unique_resource.hpp>
#include <libinneall/glad/include/glad/glad.h>
#include <libinneall/renderer/gl_buffer.hpp>

namespace inl {

class VertexArray {
public:
    struct Attribute {
        std::size_t index;
        std::size_t binding_index;
        std::size_t n_components;
        std::size_t stride_bytes;
        GLenum type;
        bool normalise;
    };

    struct BindPoint {
        std::size_t index;
        GlBuffer const& buffer;
        std::size_t offset_bytes;
        std::size_t stride_bytes;
    };

    explicit VertexArray();

    GLuint native_handle() const { return m_handle; }

    void set_attribute(Attribute const& attribute) const;

    void bind_vertex_buffer(BindPoint const& bind_point) const;
    void bind_element_buffer(GlBuffer& buffer) const;

    void bind() const { glBindVertexArray(m_handle); }
    void unbind() const { glBindVertexArray(0); }

private:
    static void delete_array(GLuint array);

    UniqueResource<GLuint, decltype(&delete_array)> m_handle { 0, delete_array };
};

} // namespace inl
