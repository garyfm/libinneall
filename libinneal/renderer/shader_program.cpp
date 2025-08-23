#include <libinneal/renderer/shader_program.hpp>

#include <libinneal/log.hpp>
#include <libinneal/renderer/shader_stage.hpp>

#include <exception>

namespace inl {

ShaderProgram::ShaderProgram(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage) {
    m_handle.reset(glCreateProgram());

    if (!m_handle) {
        throw std::runtime_error("Failed to create program");
    }

    if (!link(vertex_stage, fragment_stage)) {
        throw std::runtime_error("Failed to link program");
    }

    log::info("Created shader program id {}", m_handle.get());
}

bool ShaderProgram::link(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage) {
    glAttachShader(m_handle, vertex_stage.native_handle());
    glAttachShader(m_handle, fragment_stage.native_handle());

    glLinkProgram(m_handle);

    GLint success = 0;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &success);

    if (success != GL_TRUE) {
        std::array<GLchar, MAX_OPENGL_INFO_LOG_SIZE> info_log { 0 };
        GLsizei info_log_length { 0 };

        glGetProgramInfoLog(m_handle, MAX_OPENGL_INFO_LOG_SIZE, &info_log_length, info_log.data());
        log::error("Error compiling shader id {}: {}", m_handle.get(),
            std::string_view { info_log.data(), static_cast<std::size_t>(info_log_length) });

        return false;
    }

    return true;
}

}
