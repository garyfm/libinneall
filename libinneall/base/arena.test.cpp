#include <doctest.h>

#include <libinneall/base/arena.hpp>

TEST_CASE("Arena: Test allocation") {
    {
        uint8_t backing[100] = {};
        inl::Arena arena { backing, sizeof(backing) };
        CHECK(arena.offset() == 0);
        CHECK(arena.capacity() == sizeof(backing));
        CHECK(arena.highwater_mark() == 0);

        void* obj = arena.alloc(16);
        CHECK(obj != nullptr);
        CHECK(arena.offset() == 16);
        CHECK(arena.capacity() == sizeof(backing));
        CHECK(arena.highwater_mark() == 16);

        arena.reset();
        CHECK(arena.offset() == 0);
        CHECK(arena.capacity() == sizeof(backing));
        CHECK(arena.highwater_mark() == 16);

        obj = arena.alloc(13);
        CHECK(obj != nullptr);
        CHECK(arena.offset() == 13);
        CHECK(arena.capacity() == sizeof(backing));
        CHECK(arena.highwater_mark() == 16);

        obj = arena.alloc(4);
        CHECK(obj != nullptr);
        CHECK(arena.offset() == 20);
        CHECK(arena.capacity() == sizeof(backing));
        CHECK(arena.highwater_mark() == 20);

        arena.reset_to(13);
        CHECK(arena.offset() == 13);
        CHECK(arena.capacity() == sizeof(backing));
        CHECK(arena.highwater_mark() == 20);
    }
}
