#pragma once

#include <libinneal/base/unique_resource.hpp>
#include <libinneal/glad/include/glad/glad.h>

namespace inl {

class ShaderStage;

class ShaderProgram {
public:
    ShaderProgram(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage);

    GLuint native_handle() const { return m_handle; }

    void use() const { glUseProgram(m_handle); }

private:
    bool link(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage);

    static constexpr std::size_t MAX_OPENGL_INFO_LOG_SIZE = 512;

    UniqueResource<GLuint, decltype(glDeleteProgram)> m_handle { 0, glDeleteProgram };
};

} // namespace inl
