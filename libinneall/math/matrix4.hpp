#pragma once

#include <libinneall/base/array.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/math/matrix3.hpp>
#include <libinneall/math/vector3.hpp>
#include <libinneall/math/vector4.hpp>

namespace inl {

class Matrix4 {

public:
    Matrix4() = default;
    explicit Matrix4(float diagonal);
    explicit Matrix4(const Array<float, 16>& elements);

    float operator[](size_t i) const {
        inl_assert(i < 16, "Out of bounds array access");
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

    static Matrix4 create_scaling(Vector3 factors);
    static Matrix4 create_translation(Vector3 translation);
    static Matrix4 create_rotation(float angle_radians, Vector3 const& unit_axis);
    static Matrix4 create_perspective(float fov_y, float aspect, float z_near, float z_far);
    static Matrix4 create_look_at(Vector3 position, Vector3 target, Vector3 up);

    Span<float const> elements() const { return { m_elements.data(), m_elements.size() }; };
    float element(size_t row, size_t col) const { return m_elements[(col * 4) + row]; };

    operator Matrix3() const;

private:
    Array<float, 16> m_elements {};
};

float cofactor(Matrix4 const& matrix, size_t row, size_t col);
float determinant(Matrix4 const& matrix);
Matrix4 transpose(Matrix4 const& matrix);
Matrix4 inverse(Matrix4 const& matrix);

Vector4 operator*(Matrix4 const& matrix, Vector4 const& vector);

} // namespace inl
