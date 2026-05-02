#pragma once

#include <libinneall/base/assert.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/base/unique_handle.hpp>
#include <libinneall/base/utility.hpp>

#include <subprojects/glad/include/glad/glad.h>

namespace inl {

inline void delete_buffer(GLuint buffer) { glDeleteBuffers(1, &buffer); }

class GlBuffer {
public:
    GlBuffer() = default;
    INL_DEL_COPY_MOVE(GlBuffer)

    static Error create(GlBuffer& buffer, size_t size);
    static Error create(GlBuffer& buffer, Span<uint8_t const> data);

    void upload(Span<uint8_t const> data, size_t offset = 0);

    GLuint handle() const { return m_handle; }

    size_t size() const { return m_size; };

private:
    UniqueHandle<GLuint, delete_buffer> m_handle { 0 };
    size_t m_size { 0 };
};

} // namespace inl
