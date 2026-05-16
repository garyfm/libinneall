#pragma once

#include <libinneall/base/array.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/span.hpp>

namespace inl {

class Matrix3 {

public:
    Matrix3() = default;
    explicit Matrix3(float diagonal);
    explicit Matrix3(const Array<float, 9>& elements);

    float operator[](size_t i) const {
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

    Span<float const> elements() const { return { m_elements.data(), m_elements.size() }; };
    float element(size_t row, size_t col) const { return m_elements[(col * 3) + row]; };

private:
    Array<float, 9> m_elements {};
};

float determinant(Matrix3 const& matrix);
Matrix3 transpose(Matrix3 const& matrix);
Matrix3 inverse(Matrix3 const& matrix);

} // namespace inl
