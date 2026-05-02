#include <libinneall/base/assert.hpp>
#include <libinneall/renderer/texture.hpp>

#include <stdint.h>

namespace inl {

void delete_texture(GLuint handle) { glDeleteTextures(1, &handle); }

Error Texture::create(Texture& texture, size_t width, size_t height, uint8_t n_components, uint8_t const* data) {
    texture.m_handle.reset();
    glCreateTextures(GL_TEXTURE_2D, 1, &texture.m_handle);

    if (texture.m_handle == 0) {
        return Error::RendererTextureFailedToCreate;
    }

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

    glTextureStorage2D(texture.m_handle, 1, size_format, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    glTextureSubImage2D(texture.m_handle, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height),
        base_format, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(texture.m_handle);
    return Error::Ok;
}

void Texture::bind(GLuint texture_unit) {
    INL_ASSERT(m_handle != 0, "Invalid Texture");
    m_unit = texture_unit;
    glBindTextureUnit(m_unit, m_handle);
}

}
