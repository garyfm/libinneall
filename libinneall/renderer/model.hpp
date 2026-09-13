#pragma once

#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/transforms.hpp>
#include <libinneall/renderer/material.hpp>

namespace inl {

class Mesh;

struct Model {
    Mesh* mesh {};
    Material material {};
    Transform transform {};
};

} // namespace inl
