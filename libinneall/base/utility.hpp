#pragma once

#define INL_DEL_COPY_MOVE(T)                                                                                           \
    T(T const&) = delete;                                                                                              \
    T& operator=(T const&) = delete;                                                                                   \
    T(T&&) = delete;                                                                                                   \
    T& operator=(T&&) = delete;
