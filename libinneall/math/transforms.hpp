#pragma once

#include <libinneall/math/matrix3.hpp>
#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>
#include <libinneall/math/vector4.hpp>

namespace inl {

Vector3 scale(Vector3 vector, float scale_x, float scale_y, float scale_z);
Vector3 scale(Vector3 vector, float scale);

Matrix3 translate(const Matrix3& matrix, float scale);
Vector3 translate(Vector3 vector, float translate_x, float translate_y, float translate_z);
Matrix3 rotate(const Matrix3& matrix, float scale);

}
