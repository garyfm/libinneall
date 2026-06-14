#pragma once

#include <type_traits>

template <typename T>
concept PodType = std::is_trivially_copyable_v<T> && std::is_trivially_destructible_v<T>;

#define INL_DEL_COPY_MOVE(T)                                                                                           \
    T(T const&) = delete;                                                                                              \
    T& operator=(T const&) = delete;                                                                                   \
    T(T&&) = delete;                                                                                                   \
    T& operator=(T&&) = delete;

#define COUNTOF(array) (sizeof(array) / sizeof(array[0]))
