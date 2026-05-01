#pragma once

#include <libinneall/base/string.hpp>
#include <libinneall/base/unique_resource.hpp>
#include <libinneall/renderer/shader_stage.hpp>

#include <subprojects/glad/include/glad/glad.h>

#include <libinneall/base/string_view.hpp>
#include <unordered_map>

namespace inl {

class ShaderStage;

static constexpr size_t MAX_SHADER_UNIFORM_NAME = 128;

class ShaderProgram {
public:
    ShaderProgram() = default;

    // TODO:: static method
    void create(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage);

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram operator=(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&& other) = delete;
    ShaderProgram& operator=(ShaderProgram&& other) = delete;

    GLuint native_handle() const { return m_handle; }

    void use() const { glUseProgram(m_handle); }

    GLuint uniform_location(StringView name) const;

private:
    struct UniformInfo {
        GLint location;
        GLsizei count;
    };

    void link(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage);
    void retrieve_uniforms();

    static constexpr size_t MAX_OPENGL_INFO_LOG_SIZE = 512;

    UniqueResource<GLuint, decltype(glDeleteProgram)> m_handle { 0, glDeleteProgram };

    std::unordered_map<String<MAX_SHADER_UNIFORM_NAME>, UniformInfo, StringHash> m_uniforms;
};

} // namespace inl
