#include <doctest.h>

#include <libinneall/math/vector4.hpp>

TEST_CASE("vector4: Testing scalar operations") {

    {
        inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };

        CHECK(v1 + 2 == inl::Vector4 { 3.0f, 4.0f, 5.0f, 6.0f });
        CHECK(v1 - 1 == inl::Vector4 { 0.0f, 1.0f, 2.0f, 3.0f });
        CHECK(v1 * 2 == inl::Vector4 { 2.0f, 4.0f, 6.0f, 8.0f });
        CHECK((v1 / 2) == inl::Vector4 { 0.5f, 1.0f, 1.5f, 2.0f });

        CHECK(2 + v1 == inl::Vector4 { 3.0f, 4.0f, 5.0f, 6.0f });
        CHECK(1 - v1 == inl::Vector4 { 0.0f, 1.0f, 2.0f, 3.0f });
        CHECK(2 * v1 == inl::Vector4 { 2.0f, 4.0f, 6.0f, 8.0f });
        CHECK(2 / v1 == inl::Vector4 { 0.5f, 1.0f, 1.5f, 2.0f });

        CHECK(v1 + 2.4f == inl::Vector4 { 3.4f, 4.4f, 5.4f, 6.4f });
    }

    {
        inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
        v1 += 2;
        CHECK(v1 == inl::Vector4 { 3.0f, 4.0f, 5.0f, 6.0f });
    }

    {
        inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
        v1 -= 2;
        CHECK(v1 == inl::Vector4 { -1.0f, 0.0f, 1.0f, 2.0f });
    }

    {
        inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
        v1 *= 2;
        CHECK(v1 == inl::Vector4 { 2.0f, 4.0f, 6.0f, 8.0f });
    }

    {
        inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
        v1 /= 2;
        CHECK(v1 == inl::Vector4 { 0.5f, 1.0f, 1.5f, 2.0f });
    }
}

TEST_CASE("vector4: Testing vector operations") {

    {
        inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
        inl::Vector4 v2 { 4.0f, 5.0f, 6.0f, 7.0f };
        inl::Vector4 v3 { 2.2f, 3.3f, 4.4f, 5.5 };

        CHECK(v1 + v2 == inl::Vector4 { 5.0f, 7.0f, 9.0f, 11.0f });
        CHECK(v1 + -v2 == inl::Vector4 { -3.0f, -3.0f, -3.0f, -3.0f });

        CHECK(v2 - v1 == inl::Vector4 { 3.0f, 3.0f, 3.0f, 3.0f });
        CHECK(v1 - v2 == inl::Vector4 { -3.0f, -3.0f, -3.0f, -3.0f });

        inl::Vector4 v2_sub_v3 = v2 - v3;
        CHECK(v2_sub_v3.x == doctest::Approx(1.8));
        CHECK(v2_sub_v3.y == doctest::Approx(1.7));
        CHECK(v2_sub_v3.z == doctest::Approx(1.6));
        CHECK(v2_sub_v3.w == doctest::Approx(1.5));
    }

    {
        inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
        inl::Vector4 v2 { 4.0f, 5.0f, 6.0f, 7.0f };
        v1 += v2;
        CHECK(v1 == inl::Vector4 { 5.0f, 7.0f, 9.0f, 11.0f });
    }

    {
        inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
        inl::Vector4 v2 { 4.0f, 5.0f, 6.0f, 7.0f };
        v1 -= v2;
        CHECK(v1 == inl::Vector4 { -3.0f, -3.0f, -3.0f, -3.0f });
    }
}

TEST_CASE("vector4: Testing length") {
    inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
    CHECK(length(v1) == doctest::Approx(5.47723));
}

TEST_CASE("vector4: Testing normalise") {
    inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
    inl::Vector4 normalised { normalise(v1) };

    CHECK(normalised.x == doctest::Approx(0.182574));
    CHECK(normalised.y == doctest::Approx(0.365148));
    CHECK(normalised.z == doctest::Approx(0.547722));
    CHECK(normalised.w == doctest::Approx(0.730296));
}

TEST_CASE("vector4: Testing dot product") {
    inl::Vector4 v1 { 1.0f, 2.0f, 3.0f, 4.0f };
    inl::Vector4 v2 { 3.0f, 4.0f, 5.0f, 6.0f };

    CHECK(dot(v1, v2) == doctest::Approx(50.0f));
}
