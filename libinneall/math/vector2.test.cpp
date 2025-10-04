#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <libinneall/math/vector2.hpp>

TEST_CASE("vector2: Testing scalar operations") {

    {
        inl::Vector2 v1 { 1.0f, 2.0f };

        CHECK(v1 + 2 == inl::Vector2 { 3.0f, 4.0f });
        CHECK(v1 - 1 == inl::Vector2 { 0.0f, 1.0f });
        CHECK(v1 * 2 == inl::Vector2 { 2.0f, 4.0f });
        CHECK((v1 / 2) == inl::Vector2 { 0.5f, 1.0f });

        CHECK(2 + v1 == inl::Vector2 { 3.0f, 4.0f });
        CHECK(1 - v1 == inl::Vector2 { 0.0f, 1.0f });
        CHECK(2 * v1 == inl::Vector2 { 2.0f, 4.0f });
        CHECK(2 / v1 == inl::Vector2 { 0.5f, 1.0f });

        CHECK(v1 + 2.4 == inl::Vector2 { 3.4f, 4.4f });
    }

    {
        inl::Vector2 v1 { 1.0f, 2.0f };
        v1 += 2;
        CHECK(v1 == inl::Vector2 { 3.0f, 4.0f });
    }

    {
        inl::Vector2 v1 { 1.0f, 2.0f };
        v1 -= 2;
        CHECK(v1 == inl::Vector2 { -1.0f, 0.0f });
    }

    {
        inl::Vector2 v1 { 1.0f, 2.0f };
        v1 *= 2;
        CHECK(v1 == inl::Vector2 { 2.0f, 4.0f });
    }

    {
        inl::Vector2 v1 { 1.0f, 2.0f };
        v1 /= 2;
        CHECK(v1 == inl::Vector2 { 0.5f, 1.0f });
    }
}

TEST_CASE("vector2: Testing vector operations") {

    {
        inl::Vector2 v1 { 1.0f, 2.0f };
        inl::Vector2 v2 { 4.0f, 5.0f };
        inl::Vector2 v3 { 2.2f, 3.3f };

        CHECK(v1 + v2 == inl::Vector2 { 5.0f, 7.0f });
        CHECK(v1 + -v2 == inl::Vector2 { -3.0f, -3.0f });

        CHECK(v2 - v1 == inl::Vector2 { 3.0f, 3.0f });
        CHECK(v1 - v2 == inl::Vector2 { -3.0f, -3.0f });

        inl::Vector2 v2_sub_v3 = v2 - v3;
        CHECK(v2_sub_v3.x == doctest::Approx(1.8));
        CHECK(v2_sub_v3.y == doctest::Approx(1.7));
    }

    {
        inl::Vector2 v1 { 1.0f, 2.0f };
        inl::Vector2 v2 { 4.0f, 5.0f };
        v1 += v2;
        CHECK(v1 == inl::Vector2 { 5.0f, 7.0f });
    }

    {
        inl::Vector2 v1 { 1.0f, 2.0f };
        inl::Vector2 v2 { 4.0f, 5.0f };
        v1 -= v2;
        CHECK(v1 == inl::Vector2 { -3.0f, -3.0f });
    }
}

TEST_CASE("vector2: Testing length") {
    inl::Vector2 v1 { 1.0f, 2.0f };
    CHECK(length(v1) == doctest::Approx(2.23607));
}

TEST_CASE("vector2: Testing normalise") {
    inl::Vector2 v1 { 1.0f, 2.0f };
    inl::Vector2 normalised { normalise(v1) };

    CHECK(normalised.x == doctest::Approx(0.447213191));
    CHECK(normalised.y == doctest::Approx(0.894426382));
}

TEST_CASE("vector2: Testing dot product") {
    inl::Vector2 v1 { 1.0f, 2.0f };
    inl::Vector2 v2 { 3.0f, 4.0f };

    CHECK(dot(v1, v2) == doctest::Approx(11.0f));
}
