#include <libinneall/base/arena.hpp>
#include <libinneall/base/assert.hpp>

#include <stdlib.h>
#include <string.h>

namespace {

bool is_power_of_two(uintptr_t x) { return x != 0 && (x & (x - 1)) == 0; }

constexpr size_t align_power_of_two(size_t x, size_t alignment) { return (x + alignment - 1) & ~(alignment - 1); }

} // namespace
namespace inl {

Arena::Arena(uint8_t* backing_memory, size_t capacity)
    : m_memory { backing_memory }
    , m_capacity { capacity }
    , m_offset { 0 } {
    inl_assert(backing_memory != nullptr, "Backing memory is null");
}

void* Arena::alloc(size_t size, size_t alignment) {
    inl_assert(is_power_of_two(alignment), "Arena allocation alignment is not a power of two");
    inl_assert(m_offset + size <= m_capacity, "Arena allocation failed");

    size_t aligned_offset = align_power_of_two(m_offset, alignment);
    inl_assert(aligned_offset + size <= m_capacity, "Arena aligned allocation failed");

    uint8_t* alloc_start = m_memory + aligned_offset;
    memset(alloc_start, 0, size);

    m_offset = aligned_offset + size;

#if BUILD_DEBUG
    if (m_offset > m_highwater_mark) {
        m_highwater_mark = m_offset;
    }
#endif

    return alloc_start;
}

ArenaTemp::ArenaTemp(Arena& base_arena)
    : arena { &base_arena }
    , m_reset_to_offset { base_arena.offset() } { }

ArenaTemp::~ArenaTemp() {
    if (arena != nullptr)
        arena->reset_to(m_reset_to_offset);
}

uint8_t* allocate_backing(size_t capacity) {
    inl_assert(capacity > 0, "Invalid Arena capacity");
    // TODO: Tempory memory allocation. Should use platfrom layer
    uint8_t* backing = static_cast<uint8_t*>(calloc(capacity, sizeof(uint8_t)));
    inl_assert(backing != nullptr, "Arena creation failed to allocate backing memory");

    return backing;
}

void release_backing(uint8_t* backing) { free(backing); }
} // namespace inl
