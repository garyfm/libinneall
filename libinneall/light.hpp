#pragma once

#include <libinneall/math/vector3.hpp>

namespace inl {

struct Light {
    Vector3 pos;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
};

} // namespace inl
