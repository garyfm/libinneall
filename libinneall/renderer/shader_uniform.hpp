#pragma once

#include <libinneall/light.hpp>
#include <libinneall/math/matrix3.hpp>
#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>
#include <libinneall/renderer/color.hpp>
#include <libinneall/renderer/material.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <subprojects/glad/include/glad/glad.h>

namespace inl {

void set_uniform(ShaderProgram& shader, std::string_view name, int value);
void set_uniform(ShaderProgram& shader, std::string_view name, float value);
void set_uniform(ShaderProgram& shader, std::string_view name, GLuint value);
void set_uniform(ShaderProgram& shader, std::string_view name, Color const& color);
void set_uniform(ShaderProgram& shader, std::string_view name, Vector3 const& vector);
void set_uniform(ShaderProgram& shader, std::string_view name, Matrix3 const& matrix);
void set_uniform(ShaderProgram& shader, std::string_view name, Matrix4 const& matrix);
void set_uniform(ShaderProgram& shader, std::string_view name, Material const& material);
void set_uniform(ShaderProgram& shader, std::string_view name, LightDirectional const& light);
void set_uniform(ShaderProgram& shader, std::string_view name, LightPoint const& light);
void set_uniform(ShaderProgram& shader, std::string_view name, LightSpot const& light);

}; // namespace inl
