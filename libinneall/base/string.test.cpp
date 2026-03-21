#include <doctest.h>

#include <libinneall/base/string.hpp>

TEST_CASE("String: Test strlen") {
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

TEST_CASE("String: Test memcmp") {

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

TEST_CASE("String: Test isspace") {
    {
        char c = ' ';
        CHECK(inl::isspace(c));
        c = '\n';
        CHECK(inl::isspace(c));
        c = '\t';
        CHECK(inl::isspace(c));
    }
}

TEST_CASE("string: trim") {
    const std::string expected { "hello, world!" };
    std::string s_left { "    hello, world!" };
    std::string s_right { "hello, world!    " };
    std::string s_both { "    hello, world!    " };
    std::string s_escape { "\t    hello, world! \r\n" };

    CHECK(inl::trim_left(s_left) == expected);
    CHECK(inl::trim_right(s_right) == expected);
    CHECK(inl::trim(s_both) == expected);
    CHECK(inl::trim(s_escape) == expected);
}

TEST_CASE("string: cut") {
    {
        std::string input { "hello, world!" };
        const std::string expected_left { "hello" };
        const std::string expected_right { " world!" };

        inl::Cut result { inl::cut(input, ',') };

        CHECK(result.success);
        CHECK(result.left == expected_left);
        CHECK(result.right == expected_right);
    }

    {
        std::string input { "user=alice" };
        const std::string expected_left { "user" };
        const std::string expected_right { "alice" };

        inl::Cut result { inl::cut(input, '=') };

        CHECK(result.success);
        CHECK(result.left == expected_left);
        CHECK(result.right == expected_right);
    }

    {
        std::string input { "hello" };
        const std::string expected_left { "hello" };
        const std::string expected_right { "" };

        inl::Cut result { inl::cut(input, ' ') };

        CHECK(result.success == false);
        CHECK(result.left == expected_left);
        CHECK(result.right == expected_right);
    }
}
