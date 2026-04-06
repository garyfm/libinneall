#include <libinneall/base/string.hpp>
#include <libinneall/light.hpp>
#include <libinneall/renderer/shader_uniform.hpp>

namespace inl {

void set_uniform(ShaderProgram& shader, StringView name, uint32_t value) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniform1i(shader.native_handle(), location, value);
}

void set_uniform(ShaderProgram& shader, StringView name, int32_t value) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniform1i(shader.native_handle(), location, value);
}

void set_uniform(ShaderProgram& shader, StringView name, float value) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniform1f(shader.native_handle(), location, value);
}

void set_uniform(ShaderProgram& shader, StringView name, Color color) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniform3f(shader.native_handle(), location, color.r, color.g, color.b);
}

void set_uniform(ShaderProgram& shader, StringView name, Vector3 const& vector) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniform3f(shader.native_handle(), location, vector.x, vector.y, vector.z);
}

void set_uniform(ShaderProgram& shader, StringView name, Matrix3 const& matrix) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniformMatrix3fv(shader.native_handle(), location, 1, GL_FALSE, matrix.elements().data());
}

void set_uniform(ShaderProgram& shader, StringView name, Matrix4 const& matrix) {

    const GLuint location = shader.uniform_location(name);
    glProgramUniformMatrix4fv(shader.native_handle(), location, 1, GL_FALSE, matrix.elements().data());
}

void set_uniform(ShaderProgram& shader, StringView name, Material const& material) {
    String<MAX_SHADER_UNIFORM_NAME> buffer { name };

    // NOTE: samplers must use glProgramUniform1i, explicit cast here to ensure interger is used
    set_uniform(shader, buffer.append(".albedo"), static_cast<int32_t>(material.albedo->unit()));
    set_uniform(shader, buffer.overwrite(".specular", name.size()), static_cast<int32_t>(material.specular->unit()));
    set_uniform(shader, buffer.overwrite(".shininess", name.size()), material.shininess);
}

void set_uniform(ShaderProgram& shader, StringView name, LightDirectional const& light) {
    String<MAX_SHADER_UNIFORM_NAME> buffer { name };

    set_uniform(shader, buffer.append(".dir"), light.dir);
    set_uniform(shader, buffer.overwrite(".ambient", name.size()), light.ambient);
    set_uniform(shader, buffer.overwrite(".diffuse", name.size()), light.diffuse);
    set_uniform(shader, buffer.overwrite(".specular", name.size()), light.specular);
}

void set_uniform(ShaderProgram& shader, StringView name, LightPoint const& light) {
    String<MAX_SHADER_UNIFORM_NAME> buffer { name };

    set_uniform(shader, buffer.append(".pos"), light.pos);
    set_uniform(shader, buffer.overwrite(".ambient", name.size()), light.ambient);
    set_uniform(shader, buffer.overwrite(".diffuse", name.size()), light.diffuse);
    set_uniform(shader, buffer.overwrite(".specular", name.size()), light.specular);

    set_uniform(shader, buffer.overwrite(".atten_constant", name.size()), light.atten_constant);
    set_uniform(shader, buffer.overwrite(".atten_linear", name.size()), light.atten_linear);
    set_uniform(shader, buffer.overwrite(".atten_quadratic", name.size()), light.atten_quadratic);
}

void set_uniform(ShaderProgram& shader, StringView name, LightPoint const& light, size_t index) {
    String<MAX_STRING_SIZE_OF_NUMBER> index_str = to_string(static_cast<uint32_t>(index));
    String<MAX_SHADER_UNIFORM_NAME> buffer { name };
    buffer.append("[");
    buffer.append(index_str);
    buffer.append("]");
    size_t member_pos = buffer.size();

    set_uniform(shader, buffer.append(".pos"), light.pos);
    set_uniform(shader, buffer.overwrite(".ambient", member_pos), light.ambient);
    set_uniform(shader, buffer.overwrite(".diffuse", member_pos), light.diffuse);
    set_uniform(shader, buffer.overwrite(".specular", member_pos), light.specular);

    set_uniform(shader, buffer.overwrite(".atten_constant", member_pos), light.atten_constant);
    set_uniform(shader, buffer.overwrite(".atten_linear", member_pos), light.atten_linear);
    set_uniform(shader, buffer.overwrite(".atten_quadratic", member_pos), light.atten_quadratic);
}

void set_uniform(ShaderProgram& shader, StringView name, LightSpot const& light) {
    String<MAX_SHADER_UNIFORM_NAME> buffer { name };
    set_uniform(shader, buffer.append(".pos"), light.pos);
    set_uniform(shader, buffer.overwrite(".dir", name.size()), light.dir);
    set_uniform(shader, buffer.overwrite(".ambient", name.size()), light.ambient);
    set_uniform(shader, buffer.overwrite(".diffuse", name.size()), light.diffuse);
    set_uniform(shader, buffer.overwrite(".specular", name.size()), light.specular);
    set_uniform(shader, buffer.overwrite(".inner_cutoff_cosine", name.size()), light.inner_cutoff_cosine);
    set_uniform(shader, buffer.overwrite(".outer_cutoff_cosine", name.size()), light.outer_cutoff_cosine);
}

}; // namespace inl
