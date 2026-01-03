#pragma once

#include <libinneall/renderer/texture.hpp>

namespace inl {

struct Material {
    Texture* albedo;
    Texture* specular;
    float shininess;
};

} // namespace inl
