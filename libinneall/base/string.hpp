#pragma once

#include <libinneall/base/result.hpp>

#include <string_view>

namespace inl {

struct Cut {
    std::string_view left;
    std::string_view right;
    bool success;
};

std::string_view trim_left(std::string_view sv);
std::string_view trim_right(std::string_view sv);
std::string_view trim(std::string_view sv);

Cut cut(std::string_view sv, char deliminator);

} // namespace
