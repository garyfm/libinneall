#include <libinneall/base/array.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/math/matrix4.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/shader_stage.hpp>

namespace {

inl::Error link_shader_program(
    inl::ShaderProgram& shader_program, inl::ShaderStage const& vertex_stage, inl::ShaderStage const& fragment_stage) {
    glAttachShader(shader_program.handle(), vertex_stage.handle());
    glAttachShader(shader_program.handle(), fragment_stage.handle());

    glLinkProgram(shader_program.handle());

    GLint success { 0 };
    glGetProgramiv(shader_program.handle(), GL_LINK_STATUS, &success);

    if (success != GL_TRUE) {
        inl::String<inl::MAX_OPENGL_INFO_LOG_SIZE> info_log { inl::MAX_OPENGL_INFO_LOG_SIZE };
        GLsizei info_log_size { 0 };

        glGetProgramInfoLog(shader_program.handle(), inl::MAX_OPENGL_INFO_LOG_SIZE, &info_log_size, info_log.data());
        info_log.resize(info_log_size);
        inl::log::error("Error compiling shader id {}: {}", shader_program.handle(),
            inl::StringView { info_log.data(), info_log.size() });
        return inl::Error::RendererShaderProgramFailedToLink;
    }

    return inl::Error::Ok;
}
}

namespace inl {

Error ShaderProgram::create(
    ShaderProgram& shader_program, ShaderStage const& vertex_stage, ShaderStage const& fragment_stage) {

    shader_program.m_handle.reset(glCreateProgram());

    if (!shader_program.m_handle) {
        return Error::RendererShaderProgramFailedToCreate;
    }

    Error error = link_shader_program(shader_program, vertex_stage, fragment_stage);

    if (error != Error::Ok) {
        return error;
    }

    shader_program.retrieve_uniforms();

    log::debug("Created shader program id {}", shader_program.m_handle.get());
    return Error::Ok;
}

void ShaderProgram::retrieve_uniforms() {
    GLint uniform_count {};
    glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &uniform_count);

    log::debug("Shader({}) - Uniform count: {}", static_cast<int32_t>(m_handle), uniform_count);

    if (uniform_count == 0) {
        return;
    }

    GLint max_name_length {};
    glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length);
    log::debug("Shader({}) - Uniform max name length: {}", static_cast<int32_t>(m_handle), max_name_length);

    String<MAX_SHADER_UNIFORM_NAME> uniform_name { MAX_SHADER_UNIFORM_NAME };

    for (GLint i = 0; i < uniform_count; ++i) {

        GLsizei name_size {};
        GLsizei count {};
        GLenum type { GL_NONE };

        glGetActiveUniform(m_handle, i, max_name_length, &name_size, &count, &type, uniform_name.data());
        uniform_name.resize(name_size);

        UniformInfo info {};
        info.location = glGetUniformLocation(m_handle, uniform_name.data());
        info.count = count;
        log::debug("Shader({}) - Retrived uniform: name {}, location {}, count {}", static_cast<int32_t>(m_handle),
            uniform_name.data(), info.location, info.count);

        m_uniforms[uniform_name] = info;
    }
}

GLuint ShaderProgram::uniform_location(StringView name) const {
    // NOTE: Maybe this shouldnt assert ?
    if (m_uniforms.find(name) == m_uniforms.end()) {
        INL_ASSERT(false, "Failed to find uniform");
        return -1;
    }

    int32_t location = glGetUniformLocation(m_handle, name.data());
    INL_ASSERT(location != -1, "Failed to find uniform");
    return location;
}
}
