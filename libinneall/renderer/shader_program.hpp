#pragma once

#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_resource.hpp>
#include <libinneall/glad/include/glad/glad.h>
#include <libinneall/renderer/color.hpp>

#include <string_view>
#include <unordered_map>

namespace inl {

class ShaderStage;

class ShaderProgram {
public:
    ShaderProgram(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage);

    GLuint native_handle() const { return m_handle; }

    void use() const { glUseProgram(m_handle); }

    void set_uniform(std::string const& name, Color const& color) const;

private:
    struct UniformInfo {
        GLint location;
        GLsizei count;
    };

    void link(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage);
    void retrieve_uniforms();

    static constexpr std::size_t MAX_OPENGL_INFO_LOG_SIZE = 512;

    UniqueResource<GLuint, decltype(glDeleteProgram)> m_handle { 0, glDeleteProgram };

    std::unordered_map<std::string, UniformInfo> m_uniforms;
};

} // namespace inl
