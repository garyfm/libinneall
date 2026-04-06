#pragma once

#include <libinneall/light.hpp>
#include <libinneall/math/matrix3.hpp>
#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>
#include <libinneall/renderer/color.hpp>
#include <libinneall/renderer/material.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <libinneall/base/string_view.hpp>

namespace inl {

void set_uniform(ShaderProgram& shader, StringView name, uint32_t value);
void set_uniform(ShaderProgram& shader, StringView name, int32_t value);
void set_uniform(ShaderProgram& shader, StringView name, float value);
void set_uniform(ShaderProgram& shader, StringView name, GLuint value);
void set_uniform(ShaderProgram& shader, StringView name, Color const& color);
void set_uniform(ShaderProgram& shader, StringView name, Vector3 const& vector);
void set_uniform(ShaderProgram& shader, StringView name, Matrix3 const& matrix);
void set_uniform(ShaderProgram& shader, StringView name, Matrix4 const& matrix);
void set_uniform(ShaderProgram& shader, StringView name, Material const& material);
void set_uniform(ShaderProgram& shader, StringView name, LightDirectional const& light);
void set_uniform(ShaderProgram& shader, StringView name, LightPoint const& light);
void set_uniform(ShaderProgram& shader, StringView name, LightPoint const& light, size_t index);
void set_uniform(ShaderProgram& shader, StringView name, LightSpot const& light);

}; // namespace inl
