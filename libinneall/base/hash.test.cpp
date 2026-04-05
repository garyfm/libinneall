#include <libinneall/base/hash.hpp>

#include <doctest.h>
#include <stdint.h>

TEST_CASE("hash: fnv1a") {
    {
        char str[] = "Hello, World!";

        uint64_t hash = 0x6ef05bd7cc857c54;
        CHECK(inl::hash_fnv1a(str) == hash);
    }

    {
        std::string_view str { "test fnv1a" };

        uint64_t hash = 0x9849e0655d1498ad;
        CHECK(inl::hash_fnv1a(str) == hash);
    }
}
