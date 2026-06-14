#include <libinneall/base/hash_map.hpp>
#include <libinneall/base/string.hpp>
#include <libinneall/base/string_view.hpp>
#include <libinneall/base/utility.hpp>

#include <doctest.h>
#include <stdint.h>

using namespace inl;

uint8_t backing[4096] = { 0 };
Arena arena { backing, COUNTOF(backing) };

TEST_CASE("HashMap: Insert/Find") {

    StringView key { "key" };
    int value { 10 };

    auto map = HashMap<StringView, int>::create(arena, 10);

    map.insert(key, value);
    CHECK(map.size() == 1);
    CHECK(map.capacity() == 10);

    Option<int*> found = map.find(key);
    CHECK(found);
    CHECK(*found.value() == 10);
    CHECK(map.contains(key));

    found = map.find("not_key");
    CHECK(!found);
    CHECK(!map.contains("not_key"));
}

TEST_CASE("HashMap: Multiple elements") {

    arena.reset();

    auto map = HashMap<int, int>::create(arena, 10);

    map.insert(0, 10);
    map.insert(1, 11);
    map.insert(2, 12);
    CHECK(map.size() == 3);
    CHECK(map.capacity() == 10);

    Option<int*> found = map.find(0);
    CHECK(found);
    CHECK(*found.value() == 10);

    found = map.find(1);
    CHECK(found);
    CHECK(*found.value() == 11);

    found = map.find(2);
    CHECK(found);
    CHECK(*found.value() == 12);

    found = map.find(7);
    CHECK(!found);
}

TEST_CASE("HashMap: Duplicate key") {

    StringView key { "key" };
    int value { 10 };

    auto map = HashMap<StringView, int>::create(arena, 10);

    map.insert(key, value);

    Option<int*> found = map.find(key);
    CHECK(found);
    CHECK(*found.value() == 10);

    map.insert(key, value + 1);

    CHECK(map.size() == 1);
    CHECK(map.capacity() == 10);

    found = map.find(key);
    CHECK(found);
    CHECK(*found.value() == 11);
}

struct HasherTest {

    uint64_t operator()([[maybe_unused]] Span<uint8_t const> data) const { return 0; }

    template <PodType T> uint64_t operator()([[maybe_unused]] T data) const { return 0; }
};

TEST_CASE("HashMap: Collisions") {

    arena.reset();

    auto map = HashMap<int, int>::create(arena, 256);

    for (int i = 0; i < 100; ++i) {
        map.insert(i, i * 10);
    }

    for (int i = 0; i < 100; ++i) {
        Option<int*> found = map.find(i);
        CHECK(found);
        CHECK(*found.value() == i * 10);
    }
}

TEST_CASE("HashMap: Grow") {

    arena.reset();

    auto map = HashMap<int, int>::create(arena, 20);

    for (int i = 0; i < 100; ++i) {
        map.insert(i, i * 10);
    }

    for (int i = 0; i < 100; ++i) {
        Option<int*> found = map.find(i);
        CHECK(found);
        CHECK(*found.value() == i * 10);
    }
}

TEST_CASE("HashMap: Insert/Find String") {

    String<10> key { "key" };
    int value { 10 };

    log_debug("String test");
    auto map = HashMap<String<10>, int>::create(arena, 10);

    map.insert(key, value);
    CHECK(map.size() == 1);
    CHECK(map.capacity() == 10);

    Option<int*> found = map.find(key);
    CHECK(found);
    CHECK(*found.value() == 10);
    CHECK(map.contains(key));

    StringView key_sv { "key" };
    found = map.find(key_sv);
    CHECK(found);
    CHECK(*found.value() == 10);
    CHECK(map.contains(key));
}
