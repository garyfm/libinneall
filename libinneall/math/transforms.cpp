#include <libinneall/math/transforms.hpp>

namespace inl {

Vector3 scale(Vector3 vector, float scale_x, float scale_y, float scale_z) {

    const Vector4 result
        = Matrix4::create_scaling(scale_x, scale_y, scale_z) * Vector4 { vector.x, vector.y, vector.z, 1 };

    return { result.x, result.y, result.z };
}

Vector3 scale(Vector3 vector, float scalar) { return scale(vector, scale, scale, scale); }

Vector3 translate(Vector3 vector, float translate_x, float translate_y, float translate_z) {
    const Vector4 result = Matrix4::create_translation(translate_x, translate_y, translate_z)
        * Vector4 { vector.x, vector.y, vector.z, 1 };

    return { result.x, result.y, result.z };

    return result;
}

Vector3 translate(Vector3 vector, float translation) {
    return translate(vector, translation, translation, translation);
}
//
// Matrix3 rotate(const Matrix3& matrix, float scale) {
//     Matrix3 result {};
//     return result;
// }

} // namepsace inl
