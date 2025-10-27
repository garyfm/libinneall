#pragma once

#include <libinneall/math/matrix3.hpp>
#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>
#include <libinneall/math/vector4.hpp>

namespace inl {

Vector3 scale(Vector3 const& vector, Vector3 const& factors);
Vector3 scale(Vector3 const& vector, float factor);
Matrix4 scale(Matrix4 const& matrix, Vector3 const& factors);
Matrix4 scale(Matrix4 const& matrix, float factor);

Vector3 translate(Vector3 const& vector, Vector3 const& translation);
Vector3 translate(Vector3 const& vector, float translation);
Matrix4 translate(Matrix4 const& matrix, Vector3 const& translation);
Matrix4 translate(Matrix4 const& matrix, float translation);

Vector3 rotate(Vector3 const& vector, float angle_radians, Vector3 const& unit_axis);
Matrix4 rotate(Matrix4 const& matrix, float angle_radians, Vector3 const& unit_axis);
}
