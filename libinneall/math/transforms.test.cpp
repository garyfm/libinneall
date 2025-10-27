#include <doctest.h>

#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/transforms.hpp>
#include <libinneall/math/vector4.hpp>

TEST_CASE("matrix4: Testing scaling") {
    inl::Vector3 v1 { 1, 2, 3 };
    CHECK(inl::scale(v1, 2, 3, 4) == inl::Vector3 { 2, 8, 12 });

    inl::Vector3 v1 { 1, 2, 3 };
    CHECK(inl::scale(v1, 2) == inl::Vector3 { 2, 4, 6 });
}

TEST_CASE("matrix4: Testing translation") {

    inl::Vector3 v1 { 1, 2, 3 };
    CHECK(inl::translate(v1, 2, 3, 4) == inl::Vector3 { 3, 5, 7 });

    inl::Vector3 v1 { 1, 2, 3 };
    CHECK(inl::translate(v1, 2) == inl::Vector3 { 3, 4, 5 });
}
