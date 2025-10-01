#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <libinneall/math/vector3.hpp>

TEST_CASE("vector3: Testing scalar operations") {

    {
        inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };

        CHECK(v1 + 2 == inl::Vector3 { 3.0f, 4.0f, 5.0f });
        CHECK(v1 - 1 == inl::Vector3 { 0.0f, 1.0f, 2.0f });
        CHECK(v1 * 2 == inl::Vector3 { 2.0f, 4.0f, 6.0f });
        CHECK((v1 / 2) == inl::Vector3 { 0.5f, 1.0f, 1.5f });

        CHECK(2 + v1 == inl::Vector3 { 3.0f, 4.0f, 5.0f });
        CHECK(1 - v1 == inl::Vector3 { 0.0f, 1.0f, 2.0f });
        CHECK(2 * v1 == inl::Vector3 { 2.0f, 4.0f, 6.0f });
        CHECK(2 / v1 == inl::Vector3 { 0.5f, 1.0f, 1.5f });

        CHECK(v1 + 2.4 == inl::Vector3 { 3.4f, 4.4f, 5.4f });
    }

    {
        inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
        v1 += 2;
        CHECK(v1 == inl::Vector3 { 3.0f, 4.0f, 5.0f });
    }

    {
        inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
        v1 -= 2;
        CHECK(v1 == inl::Vector3 { -1.0f, 0.0f, 1.0f });
    }

    {
        inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
        v1 *= 2;
        CHECK(v1 == inl::Vector3 { 2.0f, 4.0f, 6.0f });
    }

    {
        inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
        v1 /= 2;
        CHECK(v1 == inl::Vector3 { 0.5f, 1.0f, 1.5f });
    }
}

TEST_CASE("vector3: Testing vector operations") {

    {
        inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
        inl::Vector3 v2 { 4.0f, 5.0f, 6.0f };
        inl::Vector3 v3 { 2.2f, 3.3f, 4.4f };

        CHECK(v1 + v2 == inl::Vector3 { 5.0f, 7.0f, 9.0f });
        CHECK(v1 + -v2 == inl::Vector3 { -3.0f, -3.0f, -3.0f });

        CHECK(v2 - v1 == inl::Vector3 { 3.0f, 3.0f, 3.0f });
        CHECK(v1 - v2 == inl::Vector3 { -3.0f, -3.0f, -3.0f });

        inl::Vector3 v2_sub_v3 = v2 - v3;
        CHECK(v2_sub_v3.x == doctest::Approx(1.8));
        CHECK(v2_sub_v3.y == doctest::Approx(1.7));
        CHECK(v2_sub_v3.z == doctest::Approx(1.6));
    }

    {
        inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
        inl::Vector3 v2 { 4.0f, 5.0f, 6.0f };
        v1 += v2;
        CHECK(v1 == inl::Vector3 { 5.0f, 7.0f, 9.0f });
    }

    {
        inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
        inl::Vector3 v2 { 4.0f, 5.0f, 6.0f };
        v1 -= v2;
        CHECK(v1 == inl::Vector3 { -3.0f, -3.0f, -3.0f });
    }
}

TEST_CASE("vector3: Testing length") {
    inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
    CHECK(length(v1) == doctest::Approx(3.74165));
}

TEST_CASE("vector3: Testing normalise") {
    inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
    inl::Vector3 normalised { normalise(v1) };

    CHECK(normalised.x == doctest::Approx(0.267261));
    CHECK(normalised.y == doctest::Approx(0.534522));
    CHECK(normalised.z == doctest::Approx(0.801783));
}

TEST_CASE("vector3: Testing dot product") {
    inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
    inl::Vector3 v2 { 3.0f, 4.0f, 5.0f };

    CHECK(dot(v1, v2) == doctest::Approx(26.0f));
}

TEST_CASE("vector3: Testing cross product") {
    inl::Vector3 v1 { 1.0f, 2.0f, 3.0f };
    inl::Vector3 v2 { 3.0f, 4.0f, 5.0f };

    CHECK(cross(v1, v2) == inl::Vector3 { -2, 4, -2 });
}
