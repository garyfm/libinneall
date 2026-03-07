#include <libinneall/base/assert.hpp>
#include <libinneall/renderer/cubemap.hpp>

namespace inl {

// TODO: This overlaps heavily with Texture. Possibly Texture could be generalised to handle multiple sub images??
Cubemap::Cubemap(size_t width, size_t height, size_t n_components, std::array<uint8_t const*, 6> faces) {

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_handle);

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

    for (size_t face = 0; face < faces.size(); ++face) {
        glTextureSubImage3D(m_handle, 0, 0, 0, static_cast<GLint>(face), static_cast<GLsizei>(width),
            static_cast<GLsizei>(height), 1, base_format, GL_UNSIGNED_BYTE, faces[face]);
    }

    // TODO: These should be parameratised
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Cubemap::Cubemap(Cubemap&& other) noexcept
    : m_handle { std::move(other.m_handle) }
    , m_unit { other.m_unit } {
    other.m_unit = 0;
}

Cubemap& Cubemap::operator=(Cubemap&& other) noexcept {

    if (this != std::addressof(other)) {
        m_handle = std::move(other.m_handle);
        m_unit = other.m_unit;

        other.m_unit = 0;
    }

    return *this;
}

void Cubemap::bind(GLuint texture_unit) {
    m_unit = texture_unit;
    glBindTextureUnit(m_unit, m_handle);
}
} // namespace inl
