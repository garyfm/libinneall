#pragma once

#include <libinneall/math/matrix4.hpp>
#include <libinneall/renderer/material.hpp>

namespace inl {

class Mesh;

struct Model {
    Mesh* mesh {};
    Material material {};
    Matrix4 model_matrix { 1 };
};

} // namespace inl
