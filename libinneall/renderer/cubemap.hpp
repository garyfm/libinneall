#pragma once

#include <libinneall/base/array.hpp>
#include <libinneall/base/unique_handle.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <stddef.h>

namespace inl {

void delete_texture(GLuint handle);

class Cubemap {
public:
    Cubemap() = default;
    void create(size_t width, size_t height, size_t n_components, Array<uint8_t const*, 6> faces);

    Cubemap(const Cubemap&) = delete;
    Cubemap operator=(const Cubemap&) = delete;

    Cubemap(Cubemap&& other) noexcept;
    Cubemap& operator=(Cubemap&& other) noexcept;

    GLuint handle() const { return m_handle; }
    void bind(GLuint texture_unit);
    GLuint unit() const { return m_unit; }

private:
    UniqueHandle<GLuint, delete_texture> m_handle { 0 };
    GLuint m_unit {};
};

} // namespace
