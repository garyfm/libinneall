#include <libinneall/base/option.hpp>
#include <libinneall/base/string_utils.hpp>
#include <libinneall/base/string_view.hpp>
#include <libinneall/math/math.hpp>

#include <doctest.h>
#include <stdint.h>

TEST_CASE("StringView: trim") {
    const inl::StringView expected { "hello, world!" };
    inl::StringView s_left { "    hello, world!" };
    inl::StringView s_right { "hello, world!    " };
    inl::StringView s_both { "    hello, world!    " };
    inl::StringView s_escape { "\t    hello, world! \r\n" };

    CHECK(inl::trim_left(s_left) == expected);
    CHECK(inl::trim_right(s_right) == expected);
    CHECK(inl::trim(s_both) == expected);
    CHECK(inl::trim(s_escape) == expected);
}

TEST_CASE("StringView: cut") {
    {
        inl::StringView input { "hello, world!" };
        const inl::StringView expected_left { "hello" };
        const inl::StringView expected_right { " world!" };

        inl::Cut result { inl::cut(input, ',') };

        CHECK(result.success);
        CHECK(result.left == expected_left);
        CHECK(result.right == expected_right);
    }

    {
        inl::StringView input { "user=alice" };
        const inl::StringView expected_left { "user" };
        const inl::StringView expected_right { "alice" };

        inl::Cut result { inl::cut(input, '=') };

        CHECK(result.success);
        CHECK(result.left == expected_left);
        CHECK(result.right == expected_right);
    }

    {
        inl::StringView input { "hello" };
        const inl::StringView expected_left { "hello" };
        // const StringView expected_right { "" };

        inl::Cut result { inl::cut(input, ' ') };

        CHECK(result.success == false);
        CHECK(result.left == expected_left);
        // CHECK(result.right == expected_right);
    }
}

TEST_CASE("StringView: find") {
    {
        inl::StringView haystack { "hello, world!" };
        inl::Option<size_t> result = haystack.find("hello");

        CHECK(result.has_value() == true);
        CHECK(*result == 0);

        result = haystack.find("world");
        CHECK(result.has_value() == true);
        CHECK(*result == 7);

        result = haystack.find(",", 4);
        CHECK(result.has_value() == true);
        CHECK(*result == 5);

        result = haystack.find("!", 10);
        CHECK(result.has_value() == true);
        CHECK(*result == 12);

        result = haystack.find("x");
        CHECK(result.has_value() == false);

        result = haystack.find("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        CHECK(result.has_value() == false);

        result = haystack.find("worldddd");
        CHECK(result.has_value() == false);
    }

    {
        inl::StringView haystack { "hello" };
        inl::Option<size_t> result = haystack.find("o");

        CHECK(result.has_value() == true);
        CHECK(*result == 4);

        result = haystack.find("oo");
        CHECK(result.has_value() == false);
    }
}

TEST_CASE("StringView: rfind") {
    {
        inl::StringView haystack { "hello, world!" };
        inl::Option<size_t> result = haystack.rfind("hello");

        CHECK(result.has_value() == true);
        CHECK(*result == 0);

        result = haystack.rfind("world");
        CHECK(result.has_value() == true);
        CHECK(*result == 7);

        result = haystack.rfind(",", 5);
        CHECK(result.has_value() == true);
        CHECK(*result == 5);

        result = haystack.rfind("!");
        CHECK(result.has_value() == true);
        CHECK(*result == 12);

        result = haystack.rfind("x");
        CHECK(result.has_value() == false);

        result = haystack.rfind("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        CHECK(result.has_value() == false);

        result = haystack.rfind("worldddd");
        CHECK(result.has_value() == false);
    }

    {
        inl::StringView haystack { "hello" };
        inl::Option<size_t> result = haystack.rfind("l");

        CHECK(result.has_value() == true);
        CHECK(*result == 3);

        result = haystack.rfind("lll");
        CHECK(result.has_value() == false);
    }

    {
        inl::StringView haystack { "../test_dir/test_file.c" };
        inl::Option<size_t> result = haystack.rfind("/");

        CHECK(result.has_value() == true);
        CHECK(*result == 11);

        result = haystack.rfind(".");
        CHECK(result.has_value() == true);
        CHECK(*result == 20);
    }
}
