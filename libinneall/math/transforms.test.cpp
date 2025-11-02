#include <doctest.h>

#include <libinneall/base/log.hpp>
#include <libinneall/math/math.hpp>
#include <libinneall/math/transforms.hpp>
#include <libinneall/math/vector3.hpp>

TEST_CASE("matrix4: Testing scaling") {
    inl::Vector3 v1 { 1, 2, 3 };
    CHECK(inl::scale(v1, { 2, 3, 4 }) == inl::Vector3 { 2, 6, 12 });

    inl::Vector3 v2 { 1, 2, 3 };
    CHECK(inl::scale(v2, 2) == inl::Vector3 { 2, 4, 6 });
}

TEST_CASE("matrix4: Testing translation") {

    inl::Vector3 v1 { 1, 2, 3 };
    CHECK(inl::translate(v1, { 2, 3, 4 }) == inl::Vector3 { 3, 5, 7 });

    inl::Vector3 v2 { 1, 2, 3 };
    CHECK(inl::translate(v2, 2) == inl::Vector3 { 3, 4, 5 });

    CHECK(inl::translate(v1, -2) == inl::Vector3 { -1, 0, 1 });
}

TEST_CASE("matrix4: Testing rotation") {

    {

        inl::Vector3 v1 { 1, 0, 0 };
        CHECK(inl::rotate(v1, inl::to_radians(90), { 0, 0, 1 }) == inl::Vector3 { 0, 1, 0 });
    }

    {
        inl::Vector3 v1 { 1, 0, 0 };
        CHECK(inl::rotate(v1, inl::to_radians(180), { 0, 1, 0 }) == inl::Vector3 { -1, 0, 0 });
    }

    {
        inl::Vector3 v1 { 0, 1, 0 };
        CHECK(inl::rotate(v1, inl::to_radians(90), { 1, 0, 0 }) == inl::Vector3 { 0, 0, 1 });
    }

    {
        inl::Vector3 v1 { 1, -2, 3 };
        CHECK(inl::rotate(v1, inl::to_radians(0), { 0, 1, 0 }) == inl::Vector3 { 1, -2, 3 });
    }

    {
        inl::Vector3 v1 { 1, 0, 0 };
        CHECK(inl::rotate(v1, inl::to_radians(120), { 0, 0, 1 }) == inl::Vector3 { -0.5, 0.866025f, 0 });
    }
}
