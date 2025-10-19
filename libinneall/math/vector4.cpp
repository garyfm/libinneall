#include <libinneall/math/vector4.hpp>

#include <cmath>

namespace inl {

Vector4 Vector4::operator-() const { return { -x, -y, -z, -w }; }

Vector4& Vector4::operator+=(float scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    w += scalar;
    return *this;
}

Vector4& Vector4::operator-=(float scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    w -= scalar;
    return *this;
}

Vector4& Vector4::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

Vector4& Vector4::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

Vector4 operator+(Vector4 const& vector, float scalar) {
    Vector4 result { vector };
    result += scalar;
    return result;
}

Vector4 operator-(Vector4 const& vector, float scalar) {
    Vector4 result { vector };
    result -= scalar;
    return result;
}

Vector4 operator*(Vector4 const& vector, float scalar) {
    Vector4 result { vector };
    result *= scalar;
    return result;
}

Vector4 operator/(Vector4 const& vector, float scalar) {
    Vector4 result { vector };
    result /= scalar;
    return result;
}

Vector4 operator+(float scalar, Vector4 const& vector) { return vector + scalar; }

Vector4 operator-(float scalar, Vector4 const& vector) { return vector - scalar; }

Vector4 operator*(float scalar, Vector4 const& vector) { return vector * scalar; }

Vector4 operator/(float scalar, Vector4 const& vector) { return vector / scalar; }

Vector4& Vector4::operator+=(Vector4 const& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Vector4& Vector4::operator-=(Vector4 const& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Vector4 operator+(Vector4 const& left, Vector4 const& right) {
    Vector4 result { left };
    result += right;
    return result;
}

Vector4 operator-(Vector4 const& left, Vector4 const& right) {
    Vector4 result { left };
    result -= right;
    return result;
}

float length(Vector4 const& vector) {
    return std::sqrt(std::pow(vector.x, 2) + std::pow(vector.y, 2) + std::pow(vector.z, 2) + std::pow(vector.w, 2));
}

Vector4 normalise(Vector4 const& vector) {
    float len = length(vector);
    return { vector.x / len, vector.y / len, vector.z / len, vector.w / len };
}

float dot(Vector4 const& vector1, Vector4 const& vector2) {
    return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z) + (vector1.w * vector2.w);
}

} // namespace inl
