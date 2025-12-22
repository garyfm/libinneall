#pragma once

#include <libinneall/math/math.hpp>

#include <cmath>
#include <format>

namespace inl {

struct Vector2 {

    union {
        struct {
            float x;
            float y;
        };

        float elements[2];
    };

    bool operator==(const Vector2& other) const {
        return std::fabs(x - other.x) < EPSILON && std::fabs(y - other.y) < EPSILON && std::fabs(y - other.y) < EPSILON;
    }

    Vector2 operator-() const;

    Vector2& operator+=(float scalar);
    Vector2& operator-=(float scalar);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);

    friend Vector2 operator+(Vector2 const& vector, float scalar);
    friend Vector2 operator-(Vector2 const& vector, float scalar);
    friend Vector2 operator*(Vector2 const& vector, float scalar);
    friend Vector2 operator/(Vector2 const& vector, float scalar);

    friend Vector2 operator+(float scalar, Vector2 const& vector);
    friend Vector2 operator-(float scalar, Vector2 const& vector);
    friend Vector2 operator*(float scalar, Vector2 const& vector);
    friend Vector2 operator/(float scalar, Vector2 const& vector);

    Vector2& operator+=(Vector2 const& other);
    Vector2& operator-=(Vector2 const& other);

    friend Vector2 operator+(Vector2 const& left, Vector2 const& right);
    friend Vector2 operator-(Vector2 const& left, Vector2 const& right);
};

float length(Vector2 const& vector);
Vector2 normalise(Vector2 const& vector);

float dot(Vector2 const& vector1, Vector2 const& vector2);

} // namespace inl
  //
template <> struct std::formatter<inl::Vector2> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const inl::Vector2& vector, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "x: {}, y: {}", vector.x, vector.y);
    }
};
