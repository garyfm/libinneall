#pragma once

#include <libinneall/math/math.hpp>

#include <cmath>
#include <format>

namespace inl {

struct Vector3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };

        float elements[3];
    };

    bool operator==(const Vector3& other) const {
        return std::fabs(x - other.x) < EPSILON && std::fabs(y - other.y) < EPSILON && std::fabs(y - other.y) < EPSILON
            && std::fabs(z - other.z) < EPSILON;
    }

    Vector3 operator-() const;

    Vector3& operator+=(float scalar);
    Vector3& operator-=(float scalar);
    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);

    friend Vector3 operator+(Vector3 const& vector, float scalar);
    friend Vector3 operator-(Vector3 const& vector, float scalar);
    friend Vector3 operator*(Vector3 const& vector, float scalar);
    friend Vector3 operator/(Vector3 const& vector, float scalar);

    friend Vector3 operator+(float scalar, Vector3 const& vector);
    friend Vector3 operator-(float scalar, Vector3 const& vector);
    friend Vector3 operator*(float scalar, Vector3 const& vector);
    friend Vector3 operator/(float scalar, Vector3 const& vector);

    Vector3& operator+=(Vector3 const& other);
    Vector3& operator-=(Vector3 const& other);

    friend Vector3 operator+(Vector3 const& left, Vector3 const& right);
    friend Vector3 operator-(Vector3 const& left, Vector3 const& right);
};

float length(Vector3 const& vector);
Vector3 normalise(Vector3 const& vector);

float dot(Vector3 const& vector1, Vector3 const& vector2);
Vector3 cross(Vector3 const& vector1, Vector3 const& vector2);

} // namespace inl
  //
template <> struct std::formatter<inl::Vector3> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const inl::Vector3& vector, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "x: {}, y: {}, z: {}", vector.x, vector.y, vector.z);
    }
};
