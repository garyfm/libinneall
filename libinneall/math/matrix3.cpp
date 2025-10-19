#include <libinneall/math/matrix3.hpp>

namespace inl {

Matrix3::Matrix3(float diagonal)
    : m_elements {
        diagonal,
        0,
        0,
        0,
        diagonal,
        0,
        0,
        0,
        diagonal,
    } { }

Matrix3::Matrix3(const std::array<float, 9>& elements)
    : m_elements { elements } { }

Matrix3 Matrix3::operator-() const {
    return Matrix3 { {
        -m_elements[0],
        -m_elements[1],
        -m_elements[2],
        -m_elements[3],
        -m_elements[4],
        -m_elements[5],
        -m_elements[6],
        -m_elements[7],
        -m_elements[8],
    } };
}

Matrix3& Matrix3::operator+=(float scalar) {

    for (auto& ele : m_elements) {
        ele += scalar;
    }

    return *this;
}

Matrix3& Matrix3::operator-=(float scalar) {
    for (auto& ele : m_elements) {
        ele -= scalar;
    }

    return *this;
}

Matrix3& Matrix3::operator*=(float scalar) {
    for (auto& ele : m_elements) {
        ele *= scalar;
    }
    return *this;
}

Matrix3& Matrix3::operator/=(float scalar) {
    for (auto& ele : m_elements) {
        ele /= scalar;
    }
    return *this;
}

Matrix3 operator+(Matrix3 const& matrix, float scalar) {
    Matrix3 result { matrix };
    result += scalar;
    return result;
}

Matrix3 operator-(Matrix3 const& matrix, float scalar) {
    Matrix3 result { matrix };
    result -= scalar;
    return result;
}

Matrix3 operator*(Matrix3 const& matrix, float scalar) {
    Matrix3 result { matrix };
    result *= scalar;
    return result;
}

Matrix3 operator/(Matrix3 const& matrix, float scalar) {
    Matrix3 result { matrix };
    result /= scalar;
    return result;
}

Matrix3 operator+(float scalar, Matrix3 const& matrix) { return matrix + scalar; }

Matrix3 operator-(float scalar, Matrix3 const& matrix) { return matrix - scalar; }

Matrix3 operator*(float scalar, Matrix3 const& matrix) { return matrix * scalar; }

Matrix3 operator/(float scalar, Matrix3 const& matrix) { return matrix / scalar; }

Matrix3& Matrix3::operator+=(Matrix3 const& other) {
    for (std::size_t i = 0; i < m_elements.size(); ++i) {
        m_elements[i] += other[i];
    }
    return *this;
}

Matrix3& Matrix3::operator-=(Matrix3 const& other) {
    for (std::size_t i = 0; i < m_elements.size(); ++i) {
        m_elements[i] -= other[i];
    }
    return *this;
}

Matrix3& Matrix3::operator*=(Matrix3 const& other) {

    Matrix3 result {};

    for (std::size_t col = 0; col < 9; col += 3) {
        for (std::size_t row = 0; row < 3; ++row) {
            result.m_elements[row + col] = m_elements[row] * other[col] + m_elements[row + 3] * other[col + 1]
                + m_elements[row + 6] * other[col + 2];
        }
    }

    *this = result;

    return *this;
}

Matrix3 operator+(Matrix3 const& left, Matrix3 const& right) {
    Matrix3 result { left };
    result += right;
    return result;
}

Matrix3 operator-(Matrix3 const& left, Matrix3 const& right) {
    Matrix3 result { left };
    result -= right;
    return result;
}

Matrix3 operator*(Matrix3 const& left, Matrix3 const& right) {
    Matrix3 result { left };
    result *= right;
    return result;
}

float determinant(Matrix3 const& matrix) {
    // Determinant 2x2
    // |a b|
    // |c d|
    // det = ad - bc

    const float det_0 = (matrix.element(1, 1) * matrix.element(2, 2)) - (matrix.element(1, 2) * matrix.element(2, 1));
    const float det_1 = (matrix.element(1, 0) * matrix.element(2, 2)) - (matrix.element(1, 2) * matrix.element(2, 0));
    const float det_2 = (matrix.element(1, 0) * matrix.element(2, 1)) - (matrix.element(1, 1) * matrix.element(2, 0));

    const float result = matrix.element(0, 0) * det_0 - matrix.element(0, 1) * det_1 + matrix.element(0, 2) * det_2;

    return result;
}

Matrix3 transpose(Matrix3 const& matrix) {
    Matrix3 result { {
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

Matrix3 inverse(Matrix3 const& matrix) {
    float det = determinant(matrix);

    Matrix3 cofactors { {
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

    Matrix3 adjoint = transpose(cofactors);

    Matrix3 result = adjoint * 1 / det;

    return result;
}

} // namespace inl
