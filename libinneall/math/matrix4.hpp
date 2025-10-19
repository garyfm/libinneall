#pragma once

#include <libinneall/base/assert.hpp>

#include <array>
#include <format>
#include <span>

namespace inl {

class Matrix4 {

public:
    Matrix4() = default;
    explicit Matrix4(float diagonal);
    explicit Matrix4(const std::array<float, 16>& elements);

    float operator[](int i) const {
        INL_ASSERT(i < 16, "Out of bounds array access");
        return m_elements[i];
    }

    bool operator==(const Matrix4& matrix) const = default;
    Matrix4 operator-() const;

    Matrix4& operator+=(float scalar);
    Matrix4& operator-=(float scalar);
    Matrix4& operator*=(float scalar);
    Matrix4& operator/=(float scalar);

    friend Matrix4 operator+(Matrix4 const& matrix, float scalar);
    friend Matrix4 operator-(Matrix4 const& matrix, float scalar);
    friend Matrix4 operator*(Matrix4 const& matrix, float scalar);
    friend Matrix4 operator/(Matrix4 const& matrix, float scalar);

    friend Matrix4 operator+(float scalar, Matrix4 const& matrix);
    friend Matrix4 operator-(float scalar, Matrix4 const& matrix);
    friend Matrix4 operator*(float scalar, Matrix4 const& matrix);
    friend Matrix4 operator/(float scalar, Matrix4 const& matrix);

    Matrix4& operator+=(Matrix4 const& other);
    Matrix4& operator-=(Matrix4 const& other);
    Matrix4& operator*=(Matrix4 const& othe);

    friend Matrix4 operator+(Matrix4 const& left, Matrix4 const& right);
    friend Matrix4 operator-(Matrix4 const& left, Matrix4 const& right);
    friend Matrix4 operator*(Matrix4 const& left, Matrix4 const& right);

    std::span<const float> elements() const { return m_elements; };
    float element(std::size_t row, std::size_t col) const { return m_elements[(col * 4) + row]; };

private:
    std::array<float, 16> m_elements {};
};

float cofactor(Matrix4 const& matrix, std::size_t row, std::size_t col);
float determinant(Matrix4 const& matrix);
Matrix4 transpose(Matrix4 const& matrix);
Matrix4 inverse(Matrix4 const& matrix);

} // namespace inl

template <> struct std::formatter<inl::Matrix4> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const inl::Matrix4& m, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}", m[0], m[4], m[8],
            m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3], m[7], m[11], m[15]);
    }
};
