#pragma once

#include <libinneall/base/unique_resource.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <stdint.h>

namespace inl {

void delete_texture(GLuint handle);

class Texture {
public:
    Texture() = default;
    explicit Texture(size_t width, size_t height, uint8_t n_components, uint8_t const* data);

    Texture(const Texture&) = delete;
    Texture operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    GLuint native_handle() const { return m_handle; }
    void bind(GLuint texture_unit);

    GLuint unit() const { return m_unit; }

private:
    UniqueResource<GLuint, decltype(&delete_texture)> m_handle { 0, delete_texture };
    GLuint m_unit {};
};

}
