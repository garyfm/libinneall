#include <libinneall/base/assert.hpp>
#include <libinneall/renderer/texture.hpp>

#include <stdint.h>

namespace inl {

void delete_texture(GLuint handle) { glDeleteTextures(1, &handle); }

void Texture::create(size_t width, size_t height, uint8_t n_components, uint8_t const* data) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);

    GLenum size_format {};
    GLenum base_format {};

    switch (n_components) {
    case 3:
        size_format = GL_RGB8;
        base_format = GL_RGB;
        break;
    case 4:
        size_format = GL_RGBA8;
        base_format = GL_RGBA;
        break;
    default:
        INL_ASSERT(false, "Number of components unsupported");
    }

    glTextureStorage2D(m_handle, 1, size_format, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glTextureSubImage2D(m_handle, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), base_format,
        GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(m_handle);
}

Texture::Texture(Texture&& other) noexcept
    : m_handle { std::move(other.m_handle) }
    , m_unit { other.m_unit } {
    other.m_unit = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {

    if (this != std::addressof(other)) {
        m_handle = std::move(other.m_handle);
        m_unit = other.m_unit;

        other.m_unit = 0;
    }

    return *this;
}

void Texture::bind(GLuint texture_unit) {
    m_unit = texture_unit;
    glBindTextureUnit(m_unit, m_handle);
}

}
