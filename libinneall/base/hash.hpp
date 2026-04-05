#pragma once

#include <libinneall/base/span.hpp>

#include <stdint.h>
#include <string_view>

namespace inl {

uint64_t hash_fnv1a(Span<uint8_t const> data);
uint64_t hash_fnv1a(std::string_view data);

}
