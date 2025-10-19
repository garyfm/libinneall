#include <libinneall/math/matrix4.hpp>

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

float determinant(Matrix4 const& matrix) {
    // Determinant 2x2
    // |a b|
    // |c d|
    // det = ad - bc

    float det_1 = (matrix[4] * matrix[8]) - (matrix[7] * matrix[5]);
    float det_2 = (matrix[1] * matrix[8]) - (matrix[7] * matrix[2]);
    float det_3 = (matrix[1] * matrix[5]) - (matrix[4] * matrix[2]);

    float result = matrix[0] * det_1 - matrix[3] * det_2 + matrix[6] * det_3;

    return result;
}

Matrix4 transpose(Matrix4 const& matrix) {
    Matrix4 result { {
        matrix[0],
        matrix[3],
        matrix[6],
        matrix[1],
        matrix[4],
        matrix[7],
        matrix[2],
        matrix[5],
        matrix[8],
    } };
    return result;
}

Matrix4 inverse(Matrix4 const& matrix) {
    float det = determinant(matrix);

    Matrix4 cofactors { {
        ((matrix[4] * matrix[8]) - (matrix[5] * matrix[7])), // 0
        -((matrix[3] * matrix[8]) - (matrix[5] * matrix[6])), // 1
        ((matrix[3] * matrix[7]) - (matrix[4] * matrix[6])), // 2
        -((matrix[1] * matrix[8]) - (matrix[2] * matrix[7])), // 3
        ((matrix[0] * matrix[8]) - (matrix[2] * matrix[6])), // 4
        -((matrix[0] * matrix[7]) - (matrix[1] * matrix[6])), // 5
        ((matrix[1] * matrix[5]) - (matrix[2] * matrix[4])), // 6
        -((matrix[0] * matrix[5]) - (matrix[2] * matrix[3])), // 7
        ((matrix[0] * matrix[4]) - (matrix[1] * matrix[3])), // 8
    } };

    Matrix4 adjoint = transpose(cofactors);

    Matrix4 result = adjoint * 1 / det;

    return result;
}

} // namespace inl
