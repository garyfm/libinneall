#include <libinneall/math/vector2.hpp>

#include <cmath>

namespace inl {

Vector2 Vector2::operator-() const { return { -x, -y }; }

Vector2& Vector2::operator+=(float scalar) {
    x += scalar;
    y += scalar;
    return *this;
}

Vector2& Vector2::operator-=(float scalar) {
    x -= scalar;
    y -= scalar;
    return *this;
}

Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

Vector2 operator+(Vector2 const& vector, float scalar) {
    Vector2 result { vector };
    result += scalar;
    return result;
}

Vector2 operator-(Vector2 const& vector, float scalar) {
    Vector2 result { vector };
    result -= scalar;
    return result;
}

Vector2 operator*(Vector2 const& vector, float scalar) {
    Vector2 result { vector };
    result *= scalar;
    return result;
}

Vector2 operator/(Vector2 const& vector, float scalar) {
    Vector2 result { vector };
    result /= scalar;
    return result;
}

Vector2 operator+(float scalar, Vector2 const& vector) { return vector + scalar; }

Vector2 operator-(float scalar, Vector2 const& vector) { return vector - scalar; }

Vector2 operator*(float scalar, Vector2 const& vector) { return vector * scalar; }

Vector2 operator/(float scalar, Vector2 const& vector) { return vector / scalar; }

Vector2& Vector2::operator+=(Vector2 const& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(Vector2 const& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2 operator+(Vector2 const& left, Vector2 const& right) {
    Vector2 result { left };
    result += right;
    return result;
}

Vector2 operator-(Vector2 const& left, Vector2 const& right) {
    Vector2 result { left };
    result -= right;
    return result;
}

float length(Vector2 const& vector) { return std::sqrt(std::pow(vector.x, 2) + std::pow(vector.y, 2)); }

Vector2 normalise(Vector2 const& vector) {
    float len = length(vector);
    return { vector.x / len, vector.y / len };
}

float dot(Vector2 const& vector1, Vector2 const& vector2) { return (vector1.x * vector2.x) + (vector1.y * vector2.y); }

} // namespace inl
