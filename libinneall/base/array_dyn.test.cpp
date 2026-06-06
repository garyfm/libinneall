#include <doctest.h>

#include <libinneall/base/arena.hpp>
#include <libinneall/base/array_dyn.hpp>

TEST_CASE("ArrayDyn: Test growth none") {
    uint8_t backing[100] = { 0 };
    inl::Arena arena { backing, sizeof(backing) };
    inl::ArrayDyn<int> array { arena, 10 };
    CHECK(array.size() == 0);
    CHECK(array.capacity() == 10);

    array.push(0);
    array.push(1);
    array.push(2);

    CHECK(array[0] == 0);
    CHECK(array[1] == 1);
    CHECK(array[2] == 2);
    CHECK(array.size() == 3);
    CHECK(array.capacity() == 10);
}

TEST_CASE("ArrayDyn: Test growth extend") {
    uint8_t backing[100] = { 0 };
    inl::Arena arena { backing, sizeof(backing) };
    inl::ArrayDyn<int> array { arena, 10 };
    CHECK(array.size() == 0);
    CHECK(array.capacity() == 10);

    for (size_t i = 0; i < array.capacity(); ++i) {
        array.push(static_cast<int>(i));
        CHECK(array[i] == i);
    }

    CHECK(array.size() == 10);
    CHECK(array.capacity() == 10);

    // Trigger extend
    array.push(11);

    CHECK(array.size() == 11);
    CHECK(array.capacity() == 11);
}

TEST_CASE("ArrayDyn: Test growth realloc") {
    uint8_t backing[200] = { 0 };
    inl::Arena arena { backing, sizeof(backing) };
    inl::ArrayDyn<int> array { arena, 10 };
    CHECK(array.size() == 0);
    CHECK(array.capacity() == 10);

    for (size_t i = 0; i < array.capacity(); ++i) {
        array.push(static_cast<int>(i));
        CHECK(array[i] == i);
    }

    CHECK(array.size() == 10);
    CHECK(array.capacity() == 10);

    arena.alloc(1);

    // Trigger realloc
    array.push(11);

    CHECK(array.size() == 11);
    CHECK(array.capacity() == 10 * 2);
}
