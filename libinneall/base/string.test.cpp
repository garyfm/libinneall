
#include <libinneall/base/string.hpp>
#include <libinneall/math/math.hpp>

#include <doctest.h>
#include <stdint.h>

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

TEST_CASE("String: Test String construction") {
    {
        inl::String str { "hello" };
        CHECK(str == "hello");
        CHECK(str.size() == inl::strlen("hello"));
        CHECK(str.capacity() == inl::strlen("hello"));
        CHECK(inl::strlen(str.data()) == inl::strlen("hello"));
    }

    {
        inl::String str1 { "hello" };
        inl::String str2 { str1 };
        CHECK(str2 == str1);
    }

    {
        inl::String<5> str1 { "hello" };
        inl::String<100> str2 { str1 };
        CHECK(str2 == str1);
    }

    {
        inl::String<100> str1 {};
        CHECK(str1.size() == 0);
        CHECK(str1.capacity() == 100);
    }

    {
        inl::String<100> str1 { "hello" };
        CHECK(str1.size() == 5);
        CHECK(str1.capacity() == 100);
    }

    {
        std::string_view sv { "hello" };
        inl::String<5> str { sv };
        CHECK(str == sv);
        CHECK(sv == str);
        CHECK(str.size() == sv.size());
        CHECK(str.capacity() == sv.size());
    }

    {
        std::string_view sv { "hello" };
        inl::String<100> str { sv };
        CHECK(str == sv);
        CHECK(sv == str);
        CHECK(str.size() == sv.size());
        CHECK(str.capacity() == 100);
    }

    {
        char c_str[] = "hello";
        inl::String str { c_str };
        CHECK(str == c_str);
        CHECK(c_str == str);
        CHECK(str.size() == inl::strlen(c_str));
        CHECK(str.capacity() == inl::strlen(c_str));
    }
}

TEST_CASE("String: Test String assignment") {
    {
        inl::String str1 { "hello" };
        inl::String str2 { "world" };

        str2 = str1;
        CHECK(str2 == str1);
        CHECK(str2.size() == str1.size());
        CHECK(str2.capacity() == str1.capacity());
    }

    {
        inl::String str1 { "hello" };
        inl::String<100> str2 { "Hello, World!" };

        str2 = str1;
        CHECK(str2 == str1);
        CHECK(str2.size() == str1.size());
        CHECK(str2.capacity() != str1.capacity());
    }
}

TEST_CASE("String: Test String accessing") {
    {
        inl::String str { "hello" };
        CHECK(str[0] == 'h');
        CHECK(str[1] == 'e');
        CHECK(str[2] == 'l');
        CHECK(str[3] == 'l');
        CHECK(str[4] == 'o');
    }

    {
        inl::String str { "hello" };
        str[0] = 'a';
        CHECK(str[0] == 'a');
        CHECK(str[1] == 'e');
        CHECK(str[2] == 'l');
        CHECK(str[3] == 'l');
        CHECK(str[4] == 'o');
    }
}
TEST_CASE("String: Test String mutating") {
    {
        inl::String<10> str {};
        str.resize(1);

        str[0] = 'a';
        CHECK(str[0] == 'a');
        CHECK(str.size() == 1);
    }

    {
        inl::String<10> str {};
        str.resize(5);

        for (char i = 0; i < static_cast<char>(str.size()); ++i) {
            str[i] = 'a' + i;
            CHECK(str[i] == 'a' + i);
            CHECK(str.size() == 5);
        }
    }

    {
        inl::String<10> str {};
        str.append("hello");

        CHECK(str[0] == 'h');
        CHECK(str[1] == 'e');
        CHECK(str[2] == 'l');
        CHECK(str[3] == 'l');
        CHECK(str[4] == 'o');
        CHECK(str.size() == 5);
        CHECK(str.capacity() == 10);
    }

    {
        inl::String<10> str { "hello" };
        str.replace("ab", 2);

        CHECK(str[0] == 'h');
        CHECK(str[1] == 'e');
        CHECK(str[2] == 'a');
        CHECK(str[3] == 'b');
        CHECK(str[4] == 'o');
        CHECK(str.size() == 5);
        CHECK(str.capacity() == 10);
    }

    {
        inl::String<10> str { "hello" };
        str.replace("world", 0);

        CHECK(str[0] == 'w');
        CHECK(str[1] == 'o');
        CHECK(str[2] == 'r');
        CHECK(str[3] == 'l');
        CHECK(str[4] == 'd');
        CHECK(str.size() == 5);
        CHECK(str.capacity() == 10);
    }

    {
        inl::String<10> str { "hello" };
        str.overwrite("abc", 1);

        CHECK(str[0] == 'h');
        CHECK(str[1] == 'a');
        CHECK(str[2] == 'b');
        CHECK(str[3] == 'c');
        CHECK(str.size() == 4);
        CHECK(str.capacity() == 10);
    }
}

TEST_CASE("String: trim") {
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

TEST_CASE("String: cut") {
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

TEST_CASE("String: to_string") {
    {

        inl::String<inl::MAX_STRING_SIZE_OF_NUMBER> str = inl::to_string(1);
        CHECK(str[0] == '1');
        CHECK(str.size() == 1);
    }

    {

        inl::String<inl::MAX_STRING_SIZE_OF_NUMBER> str = inl::to_string(0);
        CHECK(str[0] == '0');
        CHECK(str.size() == 1);
    }

    {
        inl::String<inl::MAX_STRING_SIZE_OF_NUMBER> str = inl::to_string(-1);
        CHECK(str[0] == '-');
        CHECK(str[1] == '1');
        CHECK(str.size() == 2);
    }

    {
        inl::String<inl::MAX_STRING_SIZE_OF_NUMBER> str = inl::to_string(15);
        CHECK(str[0] == '1');
        CHECK(str[1] == '5');
        CHECK(str.size() == 2);
    }
    {
        inl::String<inl::MAX_STRING_SIZE_OF_NUMBER> str = inl::to_string(-15);
        CHECK(str[0] == '-');
        CHECK(str[1] == '1');
        CHECK(str[2] == '5');
        CHECK(str.size() == 3);
    }

    {
        inl::String<inl::MAX_STRING_SIZE_OF_NUMBER> str = inl::to_string(inl::MAX_INT32);
        CHECK(str[0] == '2');
        CHECK(str[1] == '1');
        CHECK(str[2] == '4');
        CHECK(str[3] == '7');
        CHECK(str[4] == '4');
        CHECK(str[5] == '8');
        CHECK(str[6] == '3');
        CHECK(str[7] == '6');
        CHECK(str[8] == '4');
        CHECK(str[9] == '7');
        CHECK(str.size() == 10);
    }

    {
        inl::String<inl::MAX_STRING_SIZE_OF_NUMBER> str = inl::to_string(inl::MIN_INT32);
        CHECK(str[0] == '-');
        CHECK(str[1] == '2');
        CHECK(str[2] == '1');
        CHECK(str[3] == '4');
        CHECK(str[4] == '7');
        CHECK(str[5] == '4');
        CHECK(str[6] == '8');
        CHECK(str[7] == '3');
        CHECK(str[8] == '6');
        CHECK(str[9] == '4');
        CHECK(str[10] == '8');
        CHECK(str.size() == 11);
    }
    {
        inl::String<inl::MAX_STRING_SIZE_OF_NUMBER> str = inl::to_string(inl::MAX_UINT32);
        CHECK(str[0] == '4');
        CHECK(str[1] == '2');
        CHECK(str[2] == '9');
        CHECK(str[3] == '4');
        CHECK(str[4] == '9');
        CHECK(str[5] == '6');
        CHECK(str[6] == '7');
        CHECK(str[7] == '2');
        CHECK(str[8] == '9');
        CHECK(str[9] == '5');
        CHECK(str.size() == 10);
    }
}
