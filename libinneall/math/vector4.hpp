#pragma once

#include <format>

namespace inl {

struct Vector4 {
    float x;
    float y;
    float z;
    float w;

    bool operator==(const Vector4& vector) const = default;
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

    Vector4& operator+=(Vector4 const& right);
    Vector4& operator-=(Vector4 const& right);

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
