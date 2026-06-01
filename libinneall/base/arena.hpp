#pragma once

#include <libinneall/base/utility.hpp>

#include <stddef.h>
#include <stdint.h>

namespace inl {

class Arena {
public:
    Arena() = default;
    Arena(uint8_t* backing_memory, size_t size);
    INL_DEL_COPY_MOVE(Arena);

    static constexpr size_t DEFAULT_ALIGNMENT = 16;

    void* alloc(size_t size, size_t alignment = DEFAULT_ALIGNMENT);
    void reset() { m_offset = 0; }
    void reset_to(size_t pos) { m_offset = pos; }

    size_t offset() const { return m_offset; };
    size_t capacity() const { return m_capcity; };
    uint8_t const* memory() const { return m_memory; };
    uint8_t const* next_alloc() const { return m_memory + m_offset; };
#if BUILD_DEBUG
    size_t highwater_mark() const { return m_highwater_mark; };
#else
    size_t highwater_mark() const { return 0; };
#endif

private:
    uint8_t* m_memory {};
    size_t m_capcity {};
    size_t m_offset {};

#if BUILD_DEBUG
    size_t m_highwater_mark {};
#endif
};

class ArenaTemp {

public:
    ArenaTemp(Arena& base_arena);
    ~ArenaTemp();
    INL_DEL_COPY_MOVE(ArenaTemp);

    Arena* arena {};

private:
    size_t m_reset_to_offset {};
};

} // namespace inl
