#include <libinneall/base/string_utils.hpp>
#include <libinneall/math/math.hpp>

#include <doctest.h>
#include <stdint.h>

TEST_CASE("string_utils: Test strlen") {
    {
        char str[] = "Hello, World!";

        CHECK(inl::strlen(str) == 13);
    }

    {
        char str[] = "Hello\0,World!";

        CHECK(inl::strlen(str) == 5);
    }

    { CHECK(inl::strlen("Hello, World!") == 13); }
}

TEST_CASE("string_utils: Test memcmp") {

    {
        char str1[] = "Hello, World!";
        char str2[] = "Hello, World!";

        CHECK(inl::memcmp(str1, str2, inl::strlen(str1)) == 0);
    }

    {
        char str1[] = "Hello, World!";
        char str2[] = "Hfllo, World!";

        CHECK(inl::memcmp(str1, str2, inl::strlen(str1)) == -1);
    }

    {
        char str1[] = "Hello, World!";
        char str2[] = "Aello, World!";

        CHECK(inl::memcmp(str1, str2, inl::strlen(str1)) == 1);
    }

    {
        char str1[] = "Hello, World!";
        char str2[] = "Hello, Worlda";

        CHECK(inl::memcmp(str1, str2, inl::strlen(str1)) == -1);
    }
}

TEST_CASE("string_utils: Test isspace") {
    {
        char c = ' ';
        CHECK(inl::isspace(c));
        c = '\n';
        CHECK(inl::isspace(c));
        c = '\t';
        CHECK(inl::isspace(c));
    }
}
