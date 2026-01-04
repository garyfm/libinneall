#pragma once

#include <libinneall/base/unique_resource.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <cstdint>

namespace inl {
class Texture {
public:
    Texture(std::size_t width, std::size_t height, std::uint8_t n_components, std::uint8_t const* data);

    GLuint native_handle() const { return m_handle; }
    void bind(GLuint texture_unit);

    GLuint unit() const { return m_unit; }

private:
    GLuint m_unit {};
    static void delete_texture(GLuint buffer);
    UniqueResource<GLuint, decltype(&delete_texture)> m_handle { 0, delete_texture };
};

}
