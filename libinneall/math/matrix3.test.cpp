#include <doctest.h>

#include <libinneall/math/matrix3.hpp>

TEST_CASE("matrix3: Testing scalar operations") {
    {
        inl::Matrix3 m { 1 };
        CHECK(m == inl::Matrix3 { { 1, 0, 0, 0, 1, 0, 0, 0, 1 } });
    }
    {
        inl::Matrix3 m { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };

        CHECK(m + 1 == inl::Matrix3 { { 2, 5, 8, 3, 6, 9, 4, 7, 10 } });
        CHECK(m - 1 == inl::Matrix3 { { 0, 3, 6, 1, 4, 7, 2, 5, 8 } });
        CHECK(m * 2 == inl::Matrix3 { { 2, 8, 14, 4, 10, 16, 6, 12, 18 } });
        CHECK(m / 2 == inl::Matrix3 { { 0.5, 2, 3.5, 1, 2.5, 4, 1.5, 3, 4.5 } });
    }

    {
        inl::Matrix3 m { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        m += 1;
        CHECK(m == inl::Matrix3 { { 2, 5, 8, 3, 6, 9, 4, 7, 10 } });
    }
    {
        inl::Matrix3 m { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        m -= 1;
        CHECK(m == inl::Matrix3 { { 0, 3, 6, 1, 4, 7, 2, 5, 8 } });
    }
    {
        inl::Matrix3 m { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        m *= 2;
        CHECK(m == inl::Matrix3 { { 2, 8, 14, 4, 10, 16, 6, 12, 18 } });
    }
    {
        inl::Matrix3 m { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        m /= 2;
        CHECK(m == inl::Matrix3 { { 0.5, 2, 3.5, 1, 2.5, 4, 1.5, 3, 4.5 } });
    }
}

TEST_CASE("matrix3: Testing matrix operations") {
    {
        inl::Matrix3 m1 { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        inl::Matrix3 m2 { { 10, 13, 16, 11, 14, 17, 12, 15, 18 } };

        CHECK(m1 + m2 == inl::Matrix3 { { 11, 17, 23, 13, 19, 25, 15, 21, 27 } });
        CHECK(m1 - m2 == inl::Matrix3 { { -9, -9, -9, -9, -9, -9, -9, -9, -9 } });
    }

    {
        inl::Matrix3 m1 { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        inl::Matrix3 m2 { { 10, 13, 16, 11, 14, 17, 12, 15, 18 } };

        m1 += m2;
        CHECK(m1 == inl::Matrix3 { { 11, 17, 23, 13, 19, 25, 15, 21, 27 } });
    }

    {
        inl::Matrix3 m1 { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        inl::Matrix3 m2 { { 10, 13, 16, 11, 14, 17, 12, 15, 18 } };

        m1 -= m2;
        CHECK(m1 == inl::Matrix3 { { -9, -9, -9, -9, -9, -9, -9, -9, -9 } });
    }

    {
        inl::Matrix3 m1 { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        inl::Matrix3 m2 { { 10, 13, 16, 11, 14, 17, 12, 15, 18 } };

        m1 *= m2;
        CHECK(m1 == inl::Matrix3 { { 84, 201, 318, 90, 216, 342, 96, 231, 366 } });
    }

    {
        inl::Matrix3 m1 { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        inl::Matrix3 m2 { { 10, 13, 16, 11, 14, 17, 12, 15, 18 } };

        m2 *= m1;
        CHECK(m2 == inl::Matrix3 { { 138, 174, 210, 171, 216, 261, 204, 258, 312 } });
    }

    {
        inl::Matrix3 m1 { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        inl::Matrix3 m2 { { 10, 13, 16, 11, 14, 17, 12, 15, 18 } };

        inl::Matrix3 m3 = m1 * m2;
        CHECK(m3 == inl::Matrix3 { { 84, 201, 318, 90, 216, 342, 96, 231, 366 } });
    }
}

TEST_CASE("matrix3: Testing determinant") {
    {
        inl::Matrix3 m1 { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        CHECK(inl::determinant(m1) == doctest::Approx(0.0f));
    }

    {
        inl::Matrix3 m1 { { 2, 4, 9, 2, 5, 8, 3, 3, 9 } };
        CHECK(inl::determinant(m1) == doctest::Approx(-15));
    }
}

TEST_CASE("matrix3: Testing transpose") {
    {
        inl::Matrix3 m1 { { 1, 4, 7, 2, 5, 8, 3, 6, 9 } };
        CHECK(inl::transpose(m1) == inl::Matrix3 { { 1, 2, 3, 4, 5, 6, 7, 8, 9 } });
    }
}

TEST_CASE("matrix3: Testing inverse") {
    {
        inl::Matrix3 m1 { { 1, 0, 5, 2, 1, 6, 3, 4, 0 } };
        CHECK(inl::inverse(m1) == inl::Matrix3 { { -24, 20, -5, 18, -15, 4, 5, -4, 1 } });
    }
}
