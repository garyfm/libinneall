#include <libinneall/base/assert.hpp>
#include <libinneall/renderer/texture.hpp>

#include <cstdint>

namespace inl {

Texture::Texture(std::size_t width, std::size_t height, std::uint8_t n_components, std::uint8_t const* data) {
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

void Texture::bind(GLuint texture_unit) {
    m_unit = texture_unit;
    glBindTextureUnit(m_unit, m_handle);
}

void Texture::delete_texture(GLuint buffer) { glDeleteTextures(1, &buffer); }

}
