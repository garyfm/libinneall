#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <libinneall/math/vector3.hpp>

TEST_CASE("Testing vector scalar operations") {

    inl::Vector3 v1 { 1.0f, 1.0f, 1.0f };
    inl::Vector3 v2 { 3.0f, 3.0f, 3.0f };
    CHECK(v1 + 2 == v2);
}
