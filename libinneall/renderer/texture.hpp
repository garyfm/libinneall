#pragma once

#include <libinneall/base/unique_handle.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <stddef.h>
#include <stdint.h>

namespace inl {

void delete_texture(GLuint handle);

class Texture {
public:
    Texture() = default;
    void create(size_t width, size_t height, uint8_t n_components, uint8_t const* data);

    Texture(Texture const&) = delete;
    Texture operator=(Texture const&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    GLuint handle() const { return m_handle; }
    void bind(GLuint texture_unit);

    GLuint unit() const { return m_unit; }

private:
    UniqueHandle<GLuint, delete_texture> m_handle { 0 };
    GLuint m_unit {};
};

}
