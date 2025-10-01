#pragma once

namespace inl {

struct Vector3 {
    float x;
    float y;
    float z;

    friend Vector3 operator+(Vector3 const& vector, float scalar);
    friend Vector3 operator-(Vector3 const& vector, float scalar);
    friend Vector3 operator/(Vector3 const& vector, float scalar);
    friend Vector3 operator*(Vector3 const& vector, float scalar);

    friend Vector3 operator+(Vector3 const& left, Vector3 const& right);
    friend Vector3 operator-(Vector3 const& left, Vector3 const& right);
};

float length(Vector3 const& vector);
Vector3 normalise(Vector3 const& vector);

float dot(Vector3 const& vector1, Vector3 const& vector2);
Vector3 cross(Vector3 const& vector1, Vector3 const& vector2);

} // namespace inl
