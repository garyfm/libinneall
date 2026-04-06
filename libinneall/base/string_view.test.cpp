#include <libinneall/base/string.hpp>
#include <libinneall/base/string_utils.hpp>
#include <libinneall/base/string_view.hpp>
#include <libinneall/math/math.hpp>

#include <doctest.h>
#include <stdint.h>

TEST_CASE("StringView: trim") {
    const inl::String expected { "hello, world!" };
    inl::String s_left { "    hello, world!" };
    inl::String s_right { "hello, world!    " };
    inl::String s_both { "    hello, world!    " };
    inl::String s_escape { "\t    hello, world! \r\n" };

    CHECK(inl::trim_left(s_left) == expected);
    CHECK(inl::trim_right(s_right) == expected);
    CHECK(inl::trim(s_both) == expected);
    CHECK(inl::trim(s_escape) == expected);
}

TEST_CASE("StringView: cut") {
    {
        inl::String input { "hello, world!" };
        const inl::String expected_left { "hello" };
        const inl::String expected_right { " world!" };

        inl::Cut result { inl::cut(input, ',') };

        CHECK(result.success);
        CHECK(result.left == expected_left);
        CHECK(result.right == expected_right);
    }

    {
        inl::String input { "user=alice" };
        const inl::String expected_left { "user" };
        const inl::String expected_right { "alice" };

        inl::Cut result { inl::cut(input, '=') };

        CHECK(result.success);
        CHECK(result.left == expected_left);
        CHECK(result.right == expected_right);
    }

    {
        inl::String input { "hello" };
        const inl::String expected_left { "hello" };
        // const std::string_view expected_right { "" };

        inl::Cut result { inl::cut(input, ' ') };

        CHECK(result.success == false);
        CHECK(result.left == expected_left);
        // CHECK(result.right == expected_right);
    }
}
