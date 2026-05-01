#pragma once

#include <libinneall/base/log.hpp>
#include <libinneall/base/unique_handle.hpp>

#include <subprojects/glad/include/glad/glad.h>

#include <libinneall/base/string_view.hpp>

namespace inl {

enum class ShaderType { Vertex, Fragment };

inline void delete_shader(GLuint handle) { glDeleteShader(handle); }

class ShaderStage {
public:
    ShaderStage() = default;
    void create(ShaderType type, StringView source);

    ShaderStage(const ShaderStage&) = delete;
    ShaderStage operator=(const ShaderStage&) = delete;

    ShaderStage(ShaderStage&& other) = delete;
    ShaderStage& operator=(ShaderStage&& other) = delete;

    GLuint handle() const { return m_handle; }

private:
    bool compile(StringView source);

    static constexpr size_t MAX_OPENGL_INFO_LOG_SIZE = 512;

    UniqueHandle<GLuint, delete_shader> m_handle { 0 };
    ShaderType m_type {};
};

} // namespace inl
