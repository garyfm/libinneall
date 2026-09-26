#pragma once

#include <libinneall/math/vector3.hpp>

namespace inl {

class Texture;
class ShaderProgram;

struct Material {
    Texture* map_albedo {};
    Texture* map_specular {};
    Texture* map_normal {};

    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    float shininess {};

    ShaderProgram* shader {};
};

} // namespace inl
