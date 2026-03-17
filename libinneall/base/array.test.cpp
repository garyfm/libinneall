#include <doctest.h>

#include <libinneall/base/array.hpp>

TEST_CASE("Array: Test construction") {
    {
        inl::Array<int, 3> array {};

        CHECK(array[0] == 0);
        CHECK(array[1] == 0);
        CHECK(array[2] == 0);
        CHECK(array.size() == 3);
    }

    {
        inl::Array<int, 3> array { 1, 1, 1 };

        CHECK(array[0] == 1);
        CHECK(array[1] == 1);
        CHECK(array[2] == 1);
    }

    {
        inl::Array<int, 3> const array { 2, 2, 2 };

        CHECK(array[0] == 2);
        CHECK(array[1] == 2);
        CHECK(array[2] == 2);
    }

    {
        inl::Array<float, 3> const array { 1.1f, 2.2f, 3.3f };

        CHECK(array[0] == doctest::Approx(1.1f));
        CHECK(array[1] == doctest::Approx(2.2));
        CHECK(array[2] == doctest::Approx(3.3));
    }
}

TEST_CASE("Array: Test element assignment") {
    {
        inl::Array<int, 10> array;

        for (int i = 0; i < static_cast<int>(array.size()); ++i) {
            array[i] = i;
        }

        for (int i = 0; i < static_cast<int>(array.size()); ++i) {
            CHECK(array[i] == i);
        }
    }

    {
        inl::Array<int, 10> array;

        int* p = array.data();
        for (int i = 0; i < static_cast<int>(array.size()); ++i) {
            *(p + i) = i;
        }

        for (int i = 0; i < static_cast<int>(array.size()); ++i) {
            CHECK(*(p + i) == i);
        }
    }

    {
        inl::Array<int, 5> array { 1, 2, 3, 4, 5 };

        int const ele = array[0];
        CHECK(ele == 1);
    }

    {
        inl::Array<int, 5> const array { 1, 2, 3, 4, 5 };

        int const& ele = array[0];
        CHECK(ele == 1);
    }

    {
        inl::Array<int, 5> const array { 1, 2, 3, 4, 5 };

        int const* ele = &array[0];
        CHECK(*ele == 1);
    }

    {
        inl::Array<int, 5> const array { 1, 2, 3, 4, 5 };

        int const* ele = array.data();
        CHECK(*ele == 1);
    }
}
