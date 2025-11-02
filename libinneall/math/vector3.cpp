#include <libinneall/math/vector3.hpp>

#include <cmath>

namespace inl {

Vector3 Vector3::operator-() const { return { -x, -y, -z }; }

Vector3& Vector3::operator+=(float scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
}

Vector3& Vector3::operator-=(float scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    return *this;
}

Vector3& Vector3::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3& Vector3::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

Vector3 operator+(Vector3 const& vector, float scalar) {
    Vector3 result { vector };
    result += scalar;
    return result;
}

Vector3 operator-(Vector3 const& vector, float scalar) {
    Vector3 result { vector };
    result -= scalar;
    return result;
}

Vector3 operator*(Vector3 const& vector, float scalar) {
    Vector3 result { vector };
    result *= scalar;
    return result;
}

Vector3 operator/(Vector3 const& vector, float scalar) {
    Vector3 result { vector };
    result /= scalar;
    return result;
}

Vector3 operator+(float scalar, Vector3 const& vector) { return vector + scalar; }

Vector3 operator-(float scalar, Vector3 const& vector) { return vector - scalar; }

Vector3 operator*(float scalar, Vector3 const& vector) { return vector * scalar; }

Vector3 operator/(float scalar, Vector3 const& vector) { return vector / scalar; }

Vector3& Vector3::operator+=(Vector3 const& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3& Vector3::operator-=(Vector3 const& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3 operator+(Vector3 const& left, Vector3 const& right) {
    Vector3 result { left };
    result += right;
    return result;
}

Vector3 operator-(Vector3 const& left, Vector3 const& right) {
    Vector3 result { left };
    result -= right;
    return result;
}

float length(Vector3 const& vector) {
    return std::sqrtf(std::powf(vector.x, 2) + std::powf(vector.y, 2) + std::powf(vector.z, 2));
}

Vector3 normalise(Vector3 const& vector) {
    float len = length(vector);
    return { vector.x / len, vector.y / len, vector.z / len };
}

float dot(Vector3 const& vector1, Vector3 const& vector2) {
    return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);
}

Vector3 cross(Vector3 const& vector1, Vector3 const& vector2) {
    return {
        (vector1.y * vector2.z - vector1.z * vector2.y),
        (vector1.z * vector2.x - vector1.x * vector2.z),
        (vector1.x * vector2.y - vector1.y * vector2.x),
    };
}

} // namespace inl
