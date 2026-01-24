#pragma once

#include <libinneall/math/vector3.hpp>

namespace inl {

struct LightPoint {
    Vector3 pos;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;

    float atten_constant;
    float atten_linear;
    float atten_quadratic;
};

struct LightDirectional {
    Vector3 dir;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
};

} // namespace inl
