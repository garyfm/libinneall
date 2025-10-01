#include <libinneall/math/vector3.hpp>

#include <cmath>

namespace inl {

Vector3 operator+(Vector3 const& vector, float scalar) {
    return { vector.x + scalar, vector.y + scalar, vector.z + scalar };
}

Vector3 operator-(Vector3 const& vector, float scalar) {
    return { vector.x - scalar, vector.y - scalar, vector.z - scalar };
}

Vector3 operator*(Vector3 const& vector, float scalar) {
    return { vector.x * scalar, vector.y * scalar, vector.z * scalar };
}

Vector3 operator/(Vector3 const& vector, float scalar) {
    return { vector.x / scalar, vector.y / scalar, vector.z / scalar };
}

Vector3 operator+(Vector3 const& left, Vector3 const& right) {
    return { left.x + right.x, left.y + right.y, left.z + right.z };
}

Vector3 operator-(Vector3 const& left, Vector3 const& right) {
    return { left.x - right.x, left.y - right.y, left.z - right.z };
}

float length(Vector3 const& vector) {
    return std::sqrt(std::pow(vector.x, 2) + std::pow(vector.x, 2) + std::pow(vector.x, 2));
}

Vector3 normalise(Vector3 const& vector) {
    float len = length(vector);
    return { vector.x / len, vector.y / len, vector.z / len };
}

float dot(Vector3 const& vector1, Vector3 const& vector2) {
    return (vector1.x * vector2.y) + (vector1.y * vector2.y) + (vector1.z * vector2.x);
}

Vector3 cross(Vector3 const& vector1, Vector3 const& vector2) {
    return {
        (vector1.y * vector2.z - vector1.z * vector2.y),
        (vector1.z * vector2.x - vector1.x * vector2.z),
        (vector1.x * vector2.y - vector1.y * vector2.x),
    };
}

} // namespace inl
