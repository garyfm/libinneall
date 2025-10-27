#pragma once

#include <libinneall/math/math.hpp>
#include <libinneall/math/vector3.hpp>

#include <cmath>
#include <format>

namespace inl {

struct Vector4 {

    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };

        float elements[4];
    };

    explicit Vector4()
        : x {}
        , y {}
        , z {}
        , w {} { }

    Vector4(float x_in, float y_in, float z_in, float w_in)
        : x { x_in }
        , y { y_in }
        , z { z_in }
        , w { w_in } { }

    explicit Vector4(Vector3 vector)
        : x { vector.x }
        , y { vector.y }
        , z { vector.z }
        , w { 1 } { }

    bool operator==(const Vector4& other) const {
        return std::fabs(x - other.x) < EPSILON && std::fabs(y - other.y) < EPSILON && std::fabs(y - other.y) < EPSILON
            && std::fabs(z - other.z) < EPSILON && std::fabs(w - other.w) < EPSILON;
    }

    Vector4 operator-() const;

    Vector4& operator+=(float scalar);
    Vector4& operator-=(float scalar);
    Vector4& operator*=(float scalar);
    Vector4& operator/=(float scalar);

    friend Vector4 operator+(Vector4 const& vector, float scalar);
    friend Vector4 operator-(Vector4 const& vector, float scalar);
    friend Vector4 operator*(Vector4 const& vector, float scalar);
    friend Vector4 operator/(Vector4 const& vector, float scalar);

    friend Vector4 operator+(float scalar, Vector4 const& vector);
    friend Vector4 operator-(float scalar, Vector4 const& vector);
    friend Vector4 operator*(float scalar, Vector4 const& vector);
    friend Vector4 operator/(float scalar, Vector4 const& vector);

    Vector4& operator+=(Vector4 const& other);
    Vector4& operator-=(Vector4 const& other);

    friend Vector4 operator+(Vector4 const& left, Vector4 const& right);
    friend Vector4 operator-(Vector4 const& left, Vector4 const& right);
};

float length(Vector4 const& vector);
Vector4 normalise(Vector4 const& vector);

float dot(Vector4 const& vector1, Vector4 const& vector2);

} // namespace inl
  //
template <> struct std::formatter<inl::Vector4> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const inl::Vector4& vector, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "x: {}, y: {}, z: {}, w: {}", vector.x, vector.y, vector.z, vector.w);
    }
};
