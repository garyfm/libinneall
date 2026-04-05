#include <libinneall/base/hash.hpp>

namespace inl {

uint64_t hash_fnv1a(Span<uint8_t const> data) {
    static constexpr uint64_t basis { 0xcbf29ce484222325 };
    static constexpr uint64_t prime { 0x100000001b3 };

    uint64_t hash = basis;
    for (const auto& byte : data) {
        hash ^= byte;
        hash *= prime;
    }
    return hash;
}
uint64_t hash_fnv1a(std::string_view data) {
    return hash_fnv1a({ reinterpret_cast<uint8_t const*>(data.data()), data.size() });
}
}
