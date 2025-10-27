#include <libinneall/math/math.hpp>
#include <libinneall/math/transforms.hpp>

namespace inl {

Vector3 scale(Vector3 const& vector, Vector3 const& factors) {

    const Vector4 result = Matrix4::create_scaling(factors) * Vector4 { vector };

    return { result.x, result.y, result.z };
}

Vector3 scale(Vector3 const& vector, float factor) { return scale(vector, { factor, factor, factor }); }

Matrix4 scale(Matrix4 const& matrix, Vector3 const& factors) {

    const Matrix4 result = Matrix4::create_scaling(factors) * matrix;

    return result;
}

Matrix4 scale(Matrix4 const& matrix, float factor) { return scale(matrix, { factor, factor, factor }); }

Vector3 translate(Vector3 const& vector, Vector3 const& translation) {

    const Vector4 result = Matrix4::create_translation(translation) * Vector4 { vector };

    return { result.x, result.y, result.z };
}

Vector3 translate(Vector3 const& vector, float translation) {
    return translate(vector, { translation, translation, translation });
}

Matrix4 translate(Matrix4 const& matrix, Vector3 const& translation) {

    const Matrix4 result = Matrix4::create_translation(translation) * matrix;

    return result;
}

Matrix4 translate(Matrix4 const& matrix, float translation) {
    return translate(matrix, { translation, translation, translation });
}

Vector3 rotate(Vector3 const& vector, float angle_radians, Vector3 const& unit_axis) {
    const Vector4 result = Matrix4::create_rotation(angle_radians, unit_axis) * Vector4 { vector };

    return { clamp_to_zero(result.x), clamp_to_zero(result.y), clamp_to_zero(result.z) };
}

Matrix4 rotate(Matrix4 const& matrix, float angle_radians, Vector3 const& unit_axis) {
    const Matrix4 result = Matrix4::create_rotation(angle_radians, unit_axis) * matrix;

    return result;
}

} // namepsace inl
