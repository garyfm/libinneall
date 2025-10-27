#include <libinneall/math/math.hpp>
#include <libinneall/math/matrix3.hpp>
#include <libinneall/math/matrix4.hpp>

#include <array>

namespace inl {

Matrix4::Matrix4(float identity)
    : m_elements {
        identity,
        0,
        0,
        0,
        0,
        identity,
        0,
        0,
        0,
        0,
        identity,
        0,
        0,
        0,
        0,
        identity,
    } { }

Matrix4::Matrix4(const std::array<float, 16>& elements)
    : m_elements { elements } { }

Matrix4 Matrix4::operator-() const {
    return Matrix4 { {
        -m_elements[0],
        -m_elements[1],
        -m_elements[2],
        -m_elements[3],
        -m_elements[4],
        -m_elements[5],
        -m_elements[6],
        -m_elements[7],
        -m_elements[8],
        -m_elements[9],
        -m_elements[10],
        -m_elements[11],
        -m_elements[12],
        -m_elements[13],
        -m_elements[14],
        -m_elements[15],
    } };
}

Matrix4& Matrix4::operator+=(float scalar) {

    for (auto& ele : m_elements) {
        ele += scalar;
    }

    return *this;
}

Matrix4& Matrix4::operator-=(float scalar) {
    for (auto& ele : m_elements) {
        ele -= scalar;
    }

    return *this;
}

Matrix4& Matrix4::operator*=(float scalar) {
    for (auto& ele : m_elements) {
        ele *= scalar;
    }
    return *this;
}

Matrix4& Matrix4::operator/=(float scalar) {
    for (auto& ele : m_elements) {
        ele /= scalar;
    }
    return *this;
}

Matrix4 operator+(Matrix4 const& matrix, float scalar) {
    Matrix4 result { matrix };
    result += scalar;
    return result;
}

Matrix4 operator-(Matrix4 const& matrix, float scalar) {
    Matrix4 result { matrix };
    result -= scalar;
    return result;
}

Matrix4 operator*(Matrix4 const& matrix, float scalar) {
    Matrix4 result { matrix };
    result *= scalar;
    return result;
}

Matrix4 operator/(Matrix4 const& matrix, float scalar) {
    Matrix4 result { matrix };
    result /= scalar;
    return result;
}

Matrix4 operator+(float scalar, Matrix4 const& matrix) { return matrix + scalar; }

Matrix4 operator-(float scalar, Matrix4 const& matrix) { return matrix - scalar; }

Matrix4 operator*(float scalar, Matrix4 const& matrix) { return matrix * scalar; }

Matrix4 operator/(float scalar, Matrix4 const& matrix) { return matrix / scalar; }

Matrix4& Matrix4::operator+=(Matrix4 const& other) {
    for (std::size_t i = 0; i < m_elements.size(); ++i) {
        m_elements[i] += other[i];
    }
    return *this;
}

Matrix4& Matrix4::operator-=(Matrix4 const& other) {
    for (std::size_t i = 0; i < m_elements.size(); ++i) {
        m_elements[i] -= other[i];
    }
    return *this;
}

Matrix4& Matrix4::operator*=(Matrix4 const& other) {

    Matrix4 result {};

    for (std::size_t col = 0; col < 16; col += 4) {
        for (std::size_t row = 0; row < 4; ++row) {
            result.m_elements[row + col] = m_elements[row] * other[col] + m_elements[row + 4] * other[col + 1]
                + m_elements[row + 8] * other[col + 2] + m_elements[row + 12] * other[col + 3];
            ;
        }
    }

    *this = result;

    return *this;
}

Matrix4 operator+(Matrix4 const& left, Matrix4 const& right) {
    Matrix4 result { left };
    result += right;
    return result;
}

Matrix4 operator-(Matrix4 const& left, Matrix4 const& right) {
    Matrix4 result { left };
    result -= right;
    return result;
}

Matrix4 operator*(Matrix4 const& left, Matrix4 const& right) {
    Matrix4 result { left };
    result *= right;
    return result;
}

Matrix4 Matrix4::create_scaling(Vector3 factors) {
    const Matrix4 scaling { {
        factors.x,
        0,
        0,
        0,
        0,
        factors.y,
        0,
        0,
        0,
        0,
        factors.z,
        0,
        0,
        0,
        0,
        1,
    } };
    return scaling;
}

Matrix4 Matrix4::create_translation(Vector3 translation) {
    const Matrix4 translate { {
        1,
        0,
        0,
        0,
        0,
        1,
        0,
        0,
        0,
        0,
        1,
        0,
        translation.x,
        translation.y,
        translation.z,
        1,
    } };
    return translate;
}

Matrix4 Matrix4::create_rotation(float angle_radians, Vector3 const& unit_axis) {

    const float cos_angle = std::cos(angle_radians);
    const float sin_angle = std::sin(angle_radians);

    const float x = unit_axis.x;
    const float y = unit_axis.y;
    const float z = unit_axis.z;

    const Matrix4 rotation { {
        cos_angle + ((x * x) * (1 - cos_angle)),
        y * x * (1 - cos_angle) + (z * sin_angle),
        z * x * (1 - cos_angle) - (y * sin_angle),
        0,
        x * y * (1 - cos_angle) - (z * sin_angle),
        cos_angle + ((y * y) * (1 - cos_angle)),
        z * y * (1 - cos_angle) + (x * sin_angle),
        0,
        x * z * (1 - cos_angle) + (y * sin_angle),
        y * z * (1 - cos_angle) - (x * sin_angle),
        cos_angle + ((z * z) * (1 - cos_angle)),
        0,
        0,
        0,
        0,
        1,
    } };

    return rotation;
}

float cofactor(Matrix4 const& matrix, std::size_t row, std::size_t col) {

    std::array<float, 9> minor { 0 };

    std::size_t index { 0 };

    // Construct a minor matrix by removing row and col
    for (std::size_t matrix_col = 0; matrix_col < 4; ++matrix_col) {
        if (matrix_col == col) {
            continue;
        }

        for (std::size_t matrix_row = 0; matrix_row < 4; ++matrix_row) {
            if (matrix_row == row) {
                continue;
            }

            minor[index++] = matrix.element(matrix_row, matrix_col);
        }
    }

    const float det = determinant(Matrix3 { minor });
    return det * ((row + col) % 2 == 0 ? 1.0f : -1.0f);
}

float determinant(Matrix4 const& matrix) {
    // det(A) = a11*C11 + a12 * C12 + a13*C13 + a14*C14
    // Where C is the cofactor:
    // det(A) = a11*det(M11) - a12 * det(M12) + a13*det(M13) - a14*det(M14)
    // Where M is 3x3 minor after removing row and col

    const float result = matrix.element(0, 0) * cofactor(matrix, 0, 0) + matrix.element(0, 1) * cofactor(matrix, 0, 1)
        + matrix.element(0, 2) * cofactor(matrix, 0, 2) + matrix.element(0, 3) * cofactor(matrix, 0, 3);
    return result;
}

Matrix4 transpose(Matrix4 const& matrix) {
    Matrix4 result { {
        matrix[0],
        matrix[4],
        matrix[8],
        matrix[12],
        matrix[1],
        matrix[5],
        matrix[9],
        matrix[13],
        matrix[2],
        matrix[6],
        matrix[10],
        matrix[14],
        matrix[3],
        matrix[7],
        matrix[11],
        matrix[15],

    } };
    return result;
}

Matrix4 inverse(Matrix4 const& matrix) {
    float det = determinant(matrix);

    // TODO: Handle this case
    // if (det <= 0.0f) {
    //    return {};
    //}

    std::array<float, 16> cofactors { 0 };

    for (std::size_t col = 0; col < 4; ++col) {
        for (std::size_t row = 0; row < 4; ++row) {
            cofactors[(col * 4) + row] = cofactor(matrix, row, col);
        }
    }

    const Matrix4 adjoint = transpose(Matrix4 { cofactors });
    const Matrix4 result = adjoint * (1 / det);

    return result;
}

Vector4 operator*(Matrix4 const& matrix, Vector4 const& vector) {
    Vector4 result {};
    for (std::size_t row = 0; row < 4; ++row) {
        for (std::size_t col = 0; col < 4; ++col) {
            result.elements[row] += matrix.element(row, col) * vector.elements[col];
        }
    }

    return result;
}
} // namespace inl
