#pragma once

#include <libinneall/base/assert.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/base/unique_resource.hpp>

#include <subprojects/glad/include/glad/glad.h>

namespace inl {

class GlBuffer {
public:
    GlBuffer() = default;
    void create(size_t size);
    void create(Span<uint8_t const> data);

    void allocate(size_t size);
    void allocate(Span<uint8_t const> data);
    void upload(size_t offset, Span<uint8_t const> data);

    GLuint native_handle() const {
        INL_ASSERT(m_handle != 0, "Accessing invalid handle");
        return m_handle;
    }

    size_t size() const { return m_size; };

private:
    void create_buffer(uint8_t const* data, size_t size);
    static void delete_buffer(GLuint buffer);

    UniqueResource<GLuint, decltype(&delete_buffer)> m_handle { 0, delete_buffer };
    size_t m_size { 0 };
};

} // namespace inl
