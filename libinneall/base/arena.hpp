#pragma once

#include <libinneall/base/buffer.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/base/utility.hpp>

#include <stddef.h>
#include <stdint.h>

namespace inl {

struct ArenaMark {
    size_t offset;
};

class Arena {
public:
    Arena() = default;
    Arena(uint8_t* backing_memory, size_t capacity);

    static constexpr size_t DEFAULT_ALIGNMENT = 16;

    void* alloc(size_t size, size_t alignment = DEFAULT_ALIGNMENT);

    template <PodType T> T* alloc() { return static_cast<T*>(alloc(sizeof(T), alignof(T))); };

    template <PodType T> T* alloc_array(size_t count) { return static_cast<T*>(alloc(count * sizeof(T), alignof(T))); };

    template <PodType T> Buffer<T> alloc_buffer(size_t count) {
        T* data = alloc_array<T>(count);
        Buffer<T> buffer { data, count };
        return buffer;
    };

    void reset() { m_offset = 0; }
    void reset_to(size_t pos) { m_offset = pos; }
    void reset_to(ArenaMark mark) { m_offset = mark.offset; }

    size_t offset() const { return m_offset; };
    ArenaMark mark() const { return { m_offset }; };
    size_t capacity() const { return m_capacity; };
    uint8_t const* memory() const { return m_memory; };
    uint8_t const* next_alloc() const { return m_memory + m_offset; };
#if BUILD_DEBUG
    size_t highwater_mark() const { return m_highwater_mark; };
#else
    size_t highwater_mark() const { return 0; };
#endif

private:
    uint8_t* m_memory {};
    size_t m_capacity {};
    size_t m_offset {};

#if BUILD_DEBUG
    size_t m_highwater_mark {};
#endif
};

class ArenaTemp {

public:
    ArenaTemp(Arena& base_arena);
    ~ArenaTemp();

    Arena* arena {};

private:
    size_t m_reset_to_offset {};
};

uint8_t* allocate_backing(size_t capacity);
void release_backing(uint8_t* backing);

} // namespace inl
