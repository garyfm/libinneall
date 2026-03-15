#pragma once

#include <libinneall/base/unique_resource.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <array>
#include <stddef.h>

namespace inl {

void delete_texture(GLuint handle);

class Cubemap {
public:
    explicit Cubemap(size_t width, size_t height, size_t n_components, std::array<uint8_t const*, 6> faces);

    Cubemap(const Cubemap&) = delete;
    Cubemap operator=(const Cubemap&) = delete;

    Cubemap(Cubemap&& other) noexcept;
    Cubemap& operator=(Cubemap&& other) noexcept;

    GLuint native_handle() const { return m_handle; }
    void bind(GLuint texture_unit);
    GLuint unit() const { return m_unit; }

private:
    UniqueResource<GLuint, decltype(&delete_texture)> m_handle { 0, delete_texture };
    GLuint m_unit {};
};

} // namespace
