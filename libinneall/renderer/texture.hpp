#pragma once

#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_handle.hpp>
#include <libinneall/base/utility.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <stddef.h>
#include <stdint.h>

namespace inl {

void delete_texture(GLuint handle);

class Texture {
public:
    Texture() = default;
    INL_DEL_COPY_MOVE(Texture);

    static Error create(Texture& texture, size_t width, size_t height, uint8_t n_components, uint8_t const* data);

    GLuint handle() const { return m_handle; }
    void bind(GLuint texture_unit);

    GLuint unit() const { return m_unit; }

private:
    UniqueHandle<GLuint, delete_texture> m_handle { 0 };
    GLuint m_unit {};
};

}
