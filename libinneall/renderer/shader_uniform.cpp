#include "light.hpp"
#include <libinneall/renderer/shader_uniform.hpp>

namespace inl {

void set_uniform(ShaderProgram& shader, std::string_view name, int value) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniform1i(shader.native_handle(), location, value);
}

void set_uniform(ShaderProgram& shader, std::string_view name, float value) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniform1f(shader.native_handle(), location, value);
}

void set_uniform(ShaderProgram& shader, std::string_view name, Color color) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniform3f(shader.native_handle(), location, color.r, color.g, color.b);
}

void set_uniform(ShaderProgram& shader, std::string_view name, Vector3 const& vector) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniform3f(shader.native_handle(), location, vector.x, vector.y, vector.z);
}

void set_uniform(ShaderProgram& shader, std::string_view name, Matrix3 const& matrix) {
    const GLuint location = shader.uniform_location(name);
    glProgramUniformMatrix3fv(shader.native_handle(), location, 1, GL_FALSE, matrix.elements().data());
}

void set_uniform(ShaderProgram& shader, std::string_view name, Matrix4 const& matrix) {

    const GLuint location = shader.uniform_location(name);
    glProgramUniformMatrix4fv(shader.native_handle(), location, 1, GL_FALSE, matrix.elements().data());
}

void set_uniform(ShaderProgram& shader, std::string_view name, Material const& material) {
    // TODO: Replace with static string
    std::string full_name { name };
    // NOTE: samplers must use glProgramUniform1i, explicit cast here to ensure interger is ued
    set_uniform(shader, "u_material.albedo", static_cast<int>(material.albedo->unit()));
    set_uniform(shader, "u_material.specular", static_cast<int>(material.specular->unit()));
    set_uniform(shader, "u_material.shininess", material.shininess);
}

void set_uniform(ShaderProgram& shader, std::string_view name, Light const& light) {
    // TODO: Replace with static string
    std::string full_name { name };
    set_uniform(shader, full_name + ".pos", light.pos);
    set_uniform(shader, full_name + ".ambient", light.ambient);
    set_uniform(shader, full_name + ".diffuse", light.diffuse);
    set_uniform(shader, full_name + ".specular", light.specular);
}

void set_uniform(ShaderProgram& shader, std::string_view name, LightDirectional const& light) {
    // TODO: Replace with static string
    std::string full_name { name };
    set_uniform(shader, full_name + ".dir", light.dir);
    set_uniform(shader, full_name + ".ambient", light.ambient);
    set_uniform(shader, full_name + ".diffuse", light.diffuse);
    set_uniform(shader, full_name + ".specular", light.specular);
}

}; // namespace inl
