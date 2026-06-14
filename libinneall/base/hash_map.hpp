#pragma once

#include <libinneall/base/arena.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/hash.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/option.hpp>
#include <libinneall/base/span.hpp>

namespace inl {

struct HasherFnv1a {
    template <PodType T> uint64_t operator()(T const& data) const { return hash_fnv1a(as_bytes(data)); }
};

template <PodType K, PodType V, typename Hasher = HasherFnv1a> class HashMap {
public:
    HashMap() = default;

    // Return a map or take it by reference?
    static HashMap create(Arena& arena, size_t initial_capacity = DEFAULT_INITIAL_CAPACITY,
        size_t growth_factor = DEFAULT_GROWTH_FACTOR, float grow_load_factor = DEFAULT_GROW_LOAD_FACTOR) {

        inl_assert(initial_capacity * sizeof(Entry) <= arena.capacity(), "Capacity exceeds arena");
        inl_assert(initial_capacity > 0, "Invalid initial capacity");

        HashMap hash_map {};
        hash_map.m_arena = &arena;
        hash_map.m_capacity = initial_capacity;
        hash_map.m_growth_factor = growth_factor;
        hash_map.m_grow_load_factor = grow_load_factor;
        hash_map.m_buckets = hash_map.m_arena->alloc_array<Entry>(initial_capacity);
        return hash_map;
    }

    void insert(K const& key, V const& value) {
        inl_assert(m_size < m_capacity, "Exceeds capacity");

        if (load_factor() >= m_grow_load_factor) {
            grow(m_growth_factor);
        }

        insert_ele(key, value);
    }

    Option<V*> find(K const& key) const {

        uint64_t index { m_hasher(key) % m_capacity };

        size_t start { index };

        // Probe for a key
        do {
            if (!m_buckets[index].occupied) {
                return None;
            }

            if (m_buckets[index].key == key) {
                return &m_buckets[index].value;
            }

            // Wrap around
            index = (index + 1) % m_capacity;
        } while (index != start);
        return None;
    }

    bool contains(K const& key) { return find(key).has_value(); }

    void grow(size_t factor) {

        size_t old_capacity { m_capacity };
        m_capacity = { m_capacity * factor };
        inl_assert(m_capacity * sizeof(Entry) <= m_arena->capacity(), "Capacity exceeds arena");

        size_t old_size { m_size };
        Entry* old_buckets { m_buckets };
        m_buckets = m_arena->alloc_array<Entry>(m_capacity);

        m_size = 0;
        for (size_t index = 0; index < old_capacity; ++index) {
            if (!old_buckets[index].occupied) {
                continue;
            }

            insert(old_buckets[index].key, old_buckets[index].value);
        }
        inl_assert(old_size == m_size, "Rehashing lost element");
    }

    size_t size() const { return m_size; };
    size_t size_bytes() const { return m_size * sizeof(Entry); };
    size_t entry_size() const { return sizeof(Entry); };
    size_t capacity() const { return m_capacity; };
    float load_factor() const { return static_cast<float>(m_size) / static_cast<float>(m_capacity); };

private:
    static constexpr size_t DEFAULT_INITIAL_CAPACITY = 128;
    static constexpr float DEFAULT_GROW_LOAD_FACTOR = 0.8f;
    static constexpr size_t DEFAULT_GROWTH_FACTOR = 2;

    struct Entry {
        K key {};
        V value {};
        bool occupied {};
    };

    void insert_ele(K const& key, V const& value) {
        inl_assert(m_size < m_capacity, "Exceeds capacity");

        uint64_t index { m_hasher(key) % m_capacity };
        size_t start = index;

        // Probe for a free slot
        do {
            if (!m_buckets[index].occupied) {
                m_buckets[index].key = key;
                m_buckets[index].value = value;
                m_buckets[index].occupied = true;
                ++m_size;
                return;
            }

            // Update existing key
            if (m_buckets[index].key == key) {
                m_buckets[index].value = value;
                return;
            }

            // Wrap around
            index = (index + 1) % m_capacity;
        } while (index != start);
    }

    Arena* m_arena {};
    Entry* m_buckets {};
    Hasher m_hasher;
    size_t m_capacity { DEFAULT_INITIAL_CAPACITY };
    size_t m_size {};
    size_t m_growth_factor { DEFAULT_GROWTH_FACTOR };
    float m_grow_load_factor { DEFAULT_GROW_LOAD_FACTOR };
};

}; // namespace inl
