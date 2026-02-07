#pragma once

#include <libinneall/math/vector3.hpp>

namespace inl {

struct LightDirectional {
    Vector3 dir;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
};

struct LightPoint {
    Vector3 pos;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;

    float atten_constant;
    float atten_linear;
    float atten_quadratic;
};

struct LightSpot {
    Vector3 pos;
    Vector3 dir;
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    float inner_cutoff_cosine;
    float outer_cutoff_cosine;
};

} // namespace inl
