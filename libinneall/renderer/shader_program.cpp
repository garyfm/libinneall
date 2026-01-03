#include <libinneall/base/log.hpp>
#include <libinneall/math/matrix4.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/shader_stage.hpp>

namespace inl {

ShaderProgram::ShaderProgram(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage) {
    m_handle.reset(glCreateProgram());

    if (!m_handle) {
        throw std::runtime_error("Failed to create program");
    }

    link(vertex_stage, fragment_stage);

    retrieve_uniforms();

    log::debug("Created shader program id {}", m_handle.get());
}

void ShaderProgram::link(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage) {
    glAttachShader(m_handle, vertex_stage.native_handle());
    glAttachShader(m_handle, fragment_stage.native_handle());

    glLinkProgram(m_handle);

    GLint success { 0 };
    glGetProgramiv(m_handle, GL_LINK_STATUS, &success);

    if (success != GL_TRUE) {
        std::array<GLchar, MAX_OPENGL_INFO_LOG_SIZE> info_log { 0 };
        GLsizei info_log_length { 0 };

        glGetProgramInfoLog(m_handle, MAX_OPENGL_INFO_LOG_SIZE, &info_log_length, info_log.data());
        log::error("Error compiling shader id {}: {}", m_handle.get(),
            std::string_view { info_log.data(), static_cast<std::size_t>(info_log_length) });
        throw std::runtime_error("Failed to link program");
    }
}

void ShaderProgram::retrieve_uniforms() {
    GLint uniform_count {};
    glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &uniform_count);

    log::debug("Shader({}) - Uniform count: {}", static_cast<int>(m_handle), uniform_count);

    if (uniform_count == 0) {
        return;
    }

    GLint max_name_length {};
    glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_length);
    log::debug("Shader({}) - Uniform max name length: {}", static_cast<int>(m_handle), max_name_length);

    std::string uniform_name {};
    uniform_name.reserve(max_name_length);

    for (GLint i = 0; i < uniform_count; ++i) {

        GLsizei name_length {};
        GLsizei count {};
        GLenum type { GL_NONE };
        glGetActiveUniform(m_handle, i, max_name_length, &name_length, &count, &type, uniform_name.data());

        UniformInfo info {};
        info.location = glGetUniformLocation(m_handle, uniform_name.c_str());
        info.count = count;
        log::debug("Shader({}) - Retrived uniform: name {}, location {}, count {}", static_cast<int>(m_handle),
            uniform_name.c_str(), info.location, info.count);

        m_uniforms.emplace(std::make_pair(uniform_name.c_str(), info));
        uniform_name.clear();
    }
}

GLuint ShaderProgram::uniform_location(std::string_view name) const {

    if (m_uniforms.find(name.data()) == m_uniforms.end()) {
        log::error("Shader({}) - Failed to find uniform with name {}", static_cast<int>(m_handle), name.data());
        throw std::runtime_error("Failed to find uniform");
    }

    int location = glGetUniformLocation(m_handle, name.data());
    return location;
}

void ShaderProgram::set_uniform(std::string_view name, int value) const {
    const GLuint location = uniform_location(name);
    glProgramUniform1i(m_handle, location, value);
}

void ShaderProgram::set_uniform(std::string_view name, float value) const {
    const GLuint location = uniform_location(name);
    glProgramUniform1f(m_handle, location, value);
}

void ShaderProgram::set_uniform(std::string_view name, Color const& color) const {
    const GLuint location = uniform_location(name);
    glProgramUniform3f(m_handle, location, color.r, color.g, color.b);
}

void ShaderProgram::set_uniform(std::string_view name, Vector3 const& vector) const {
    const GLuint location = uniform_location(name);
    glProgramUniform3f(m_handle, location, vector.x, vector.y, vector.z);
}

void ShaderProgram::set_uniform(std::string_view name, Matrix3 const& matrix) const {

    const GLuint location = uniform_location(name);
    glProgramUniformMatrix3fv(m_handle, location, 1, GL_FALSE, matrix.elements().data());
}

void ShaderProgram::set_uniform(std::string_view name, Matrix4 const& matrix) const {

    const GLuint location = uniform_location(name);
    glProgramUniformMatrix4fv(m_handle, location, 1, GL_FALSE, matrix.elements().data());
}

void ShaderProgram::set_uniform(std::string_view name, Material const& material) const {
    // TODO: Replace with static string
    std::string full_name { name };
    // NOTE: samplers must use glProgramUniform1i, explicit cast here to ensure interger is ued
    set_uniform("u_material.albedo", static_cast<int>(material.albedo->unit()));
    set_uniform("u_material.specular", static_cast<int>(material.specular->unit()));
    set_uniform("u_material.shininess", material.shininess);
}
void ShaderProgram::set_uniform(std::string_view name, Light const& light) const {
    // TODO: Replace with static string
    std::string full_name { name };
    set_uniform(full_name + ".pos", light.pos);
    set_uniform(full_name + ".ambient", light.ambient);
    set_uniform(full_name + ".diffuse", light.diffuse);
    set_uniform(full_name + ".specular", light.specular);
}
}
