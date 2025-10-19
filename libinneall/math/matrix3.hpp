#pragma once

#include <libinneall/base/assert.hpp>

#include <array>
#include <format>
#include <span>

namespace inl {

class Matrix3 {

public:
    Matrix3() = default;
    explicit Matrix3(float diagonal);
    explicit Matrix3(const std::array<float, 9>& elements);

    float operator[](int i) const {
        INL_ASSERT(i < 9, "Out of bounds array access");
        return m_elements[i];
    }

    bool operator==(const Matrix3& matrix) const = default;
    Matrix3 operator-() const;

    Matrix3& operator+=(float scalar);
    Matrix3& operator-=(float scalar);
    Matrix3& operator*=(float scalar);
    Matrix3& operator/=(float scalar);

    friend Matrix3 operator+(Matrix3 const& matrix, float scalar);
    friend Matrix3 operator-(Matrix3 const& matrix, float scalar);
    friend Matrix3 operator*(Matrix3 const& matrix, float scalar);
    friend Matrix3 operator/(Matrix3 const& matrix, float scalar);

    friend Matrix3 operator+(float scalar, Matrix3 const& matrix);
    friend Matrix3 operator-(float scalar, Matrix3 const& matrix);
    friend Matrix3 operator*(float scalar, Matrix3 const& matrix);
    friend Matrix3 operator/(float scalar, Matrix3 const& matrix);

    Matrix3& operator+=(Matrix3 const& other);
    Matrix3& operator-=(Matrix3 const& other);
    Matrix3& operator*=(Matrix3 const& othe);

    friend Matrix3 operator+(Matrix3 const& left, Matrix3 const& right);
    friend Matrix3 operator-(Matrix3 const& left, Matrix3 const& right);
    friend Matrix3 operator*(Matrix3 const& left, Matrix3 const& right);

    std::span<const float> elements() const { return m_elements; };
    float element(std::size_t row, std::size_t col) const { return m_elements[(col * 3) + row]; };

private:
    std::array<float, 9> m_elements {};
};

float determinant(Matrix3 const& matrix);
Matrix3 transpose(Matrix3 const& matrix);
Matrix3 inverse(Matrix3 const& matrix);

} // namespace inl

template <> struct std::formatter<inl::Matrix3> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const inl::Matrix3& m, std::format_context& ctx) const {
        return std::format_to(
            ctx.out(), "\n{} {} {}\n{} {} {}\n{} {} {}", m[0], m[3], m[6], m[1], m[4], m[7], m[2], m[5], m[8]);
    }
};
