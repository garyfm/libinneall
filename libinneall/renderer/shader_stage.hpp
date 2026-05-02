#pragma once

#include <libinneall/base/log.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_handle.hpp>
#include <libinneall/base/utility.hpp>

#include <subprojects/glad/include/glad/glad.h>

#include <libinneall/base/string_view.hpp>

namespace inl {

enum class ShaderType { Vertex, Fragment };

inline void delete_shader(GLuint handle) { glDeleteShader(handle); }

static constexpr size_t MAX_OPENGL_INFO_LOG_SIZE = 512;

class ShaderStage {
public:
    ShaderStage() = default;

    INL_DEL_COPY_MOVE(ShaderStage);

    static Error create(ShaderStage& shader_stage, ShaderType type, StringView source);

    GLuint handle() const { return m_handle; }

private:
    UniqueHandle<GLuint, delete_shader> m_handle { 0 };
    ShaderType m_type {};
};

} // namespace inl
