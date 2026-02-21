#pragma once

#include <libinneall/base/unique_resource.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <span>

namespace inl {

class GlBuffer {
public:
    GlBuffer() = default;
    explicit GlBuffer(std::size_t size);
    explicit GlBuffer(std::span<std::byte const> data);

    void allocate(std::size_t size);
    void allocate(std::span<std::byte const> data);
    void upload(std::size_t offset, std::span<std::byte const> data);

    GLuint native_handle() const { return m_handle; }
    std::size_t size() const { return m_size; };

private:
    void create_buffer(std::byte const* data, std::size_t size);
    static void delete_buffer(GLuint buffer);

    UniqueResource<GLuint, decltype(&delete_buffer)> m_handle { 0, delete_buffer };
    std::size_t m_size { 0 };
};

} // namespace inl
