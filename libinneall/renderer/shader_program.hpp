#pragma once

#include <libinneall/base/string.hpp>
#include <libinneall/base/unique_resource.hpp>
#include <libinneall/renderer/shader_stage.hpp>

#include <subprojects/glad/include/glad/glad.h>

#include <string_view>
#include <unordered_map>

namespace inl {

class ShaderStage;

static constexpr size_t MAX_SHADER_UNIFORM_NAME = 128;

class ShaderProgram {
public:
    ShaderProgram(ShaderStage vertex_stage, ShaderStage fragment_stage);

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    GLuint native_handle() const { return m_handle; }

    void use() const { glUseProgram(m_handle); }

    GLuint uniform_location(std::string_view name) const;

private:
    struct UniformInfo {
        GLint location;
        GLsizei count;
    };

    void link(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage);
    void retrieve_uniforms();

    static constexpr size_t MAX_OPENGL_INFO_LOG_SIZE = 512;

    UniqueResource<GLuint, decltype(glDeleteProgram)> m_handle { 0, glDeleteProgram };

    ShaderStage m_vertex {};
    ShaderStage m_fragment {};

    std::unordered_map<String<MAX_SHADER_UNIFORM_NAME>, UniformInfo, StringHash> m_uniforms;
};

} // namespace inl
