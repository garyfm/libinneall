#include <libinneall/base/string.hpp>

#include <algorithm>

namespace inl {

void trim_left(std::string& str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isspace(c); }));
}

void trim_right(std::string& str) {
    str.erase(
        std::find_if(str.rbegin(), str.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), str.end());
}

void trim(std::string& str) {
    trim_left(str);
    trim_right(str);
}

} // namepace inl
