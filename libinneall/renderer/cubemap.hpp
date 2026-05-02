#pragma once

#include <libinneall/base/array.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_handle.hpp>
#include <libinneall/base/utility.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <stddef.h>

namespace inl {

void delete_texture(GLuint handle);

class Cubemap {
public:
    Cubemap() = default;

    static Error create(
        Cubemap& cubemap, size_t width, size_t height, size_t n_components, Array<uint8_t const*, 6> faces);

    INL_DEL_COPY_MOVE(Cubemap);

    GLuint handle() const { return m_handle; }
    void bind(GLuint texture_unit);
    GLuint unit() const { return m_unit; }

private:
    UniqueHandle<GLuint, delete_texture> m_handle { 0 };
    GLuint m_unit {};
};

} // namespace
