#pragma once

#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_resource.hpp>
#include <libinneall/math/matrix4.hpp>
#include <libinneall/renderer/color.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <string_view>
#include <unordered_map>

namespace inl {

class ShaderStage;

class ShaderProgram {
public:
    ShaderProgram(ShaderStage const& vertex_stage, ShaderStage const& fragment_stage);

    GLuint native_handle() const { return m_handle; }

    void use() const { glUseProgram(m_handle); }

    void set_uniform(std::string_view name, int value) const;
    void set_uniform(std::string_view name, Color const& color) const;
    void set_uniform(std::string_view name, Vector3 const& vector) const;
    void set_uniform(std::string_view name, Matrix3 const& matrix) const;
    void set_uniform(std::string_view name, Matrix4 const& color) const;

    GLuint uniform_location(std::string_view name) const;

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
