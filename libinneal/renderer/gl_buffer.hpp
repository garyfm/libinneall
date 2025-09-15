#pragma once

#include <libinneal/base/unique_resource.hpp>
#include <libinneal/glad/include/glad/glad.h>

#include <span>

namespace inl {

class GlBuffer {
public:
    GlBuffer() = default;
    explicit GlBuffer(std::span<std::byte const> data);

    GLuint native_handle() const { return m_handle; }
    std::size_t size() const { return m_data.size(); };

private:
    static void delete_buffer(GLuint buffer);

    UniqueResource<GLuint, decltype(&delete_buffer)> m_handle { 0, delete_buffer };
    std::span<std::byte const> m_data {};
};

} // namespace inl
