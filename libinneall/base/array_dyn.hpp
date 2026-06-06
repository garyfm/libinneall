#pragma once

#include <libinneall/base/arena.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/span.hpp>
#include <libinneall/base/string_utils.hpp>
#include <libinneall/base/utility.hpp>

namespace inl {
template <PodType T> class ArrayDyn {
public:
    ArrayDyn(Arena& arena, size_t initial_capacity, size_t growth_factor = 2)
        : m_arena { &arena }
        , m_data {}
        , m_size {}
        , m_capacity { initial_capacity }
        , m_growth_factor { growth_factor } {
        inl_assert(growth_factor >= 2, "Invalid growth factor");
        m_data = m_arena->alloc_array<T>(m_capacity);
    };

    INL_DEL_COPY_MOVE(ArrayDyn);

    void push(T const& ele) {
        inl_assert(m_arena != nullptr, "Invalid arena");

        if (m_size < m_capacity) {
            // Space in array
            m_data[m_size] = ele;
        } else if (reinterpret_cast<uintptr_t>(m_arena->next_alloc()) == reinterpret_cast<uintptr_t>(end())) {
            // Nothing else has been pushed to the arena so it is safe to extended it
            m_arena->alloc(sizeof(T));
            ++m_capacity;
            m_data[m_size] = ele;
        } else {
            // Realloc array
            size_t new_capacity = m_capacity == 0 ? 1 : m_capacity * m_growth_factor;
            T* new_memory = m_arena->alloc_array<T>(new_capacity);
            m_capacity = new_capacity;

            memcpy(new_memory, m_data, m_size * sizeof(T));
            m_data = new_memory;
            m_data[m_size] = ele;
        }
        ++m_size;
    }

    T& operator[](size_t index) {
        inl_assert(index < m_size, "Out of bounds access");
        return m_data[index];
    }

    T const& operator[](size_t index) const {
        inl_assert(index < m_size, "Out of bounds access");
        return m_data[index];
    }

    operator Span<T>() { return { m_data, m_size }; }

    T* data() { return m_data; };
    T const* data() const { return m_data; };

    T* begin() { return m_data; };
    T const* begin() const { return m_data; };

    T* end() { return m_data + m_size; };
    T const* end() const { return m_data + m_size; };

    size_t size() const { return m_size; };
    size_t capacity() const { return m_capacity; };

private:
    Arena* m_arena {};
    T* m_data {};
    size_t m_size {};
    size_t m_capacity {};
    size_t m_growth_factor {};
};

}; // namespace inl
