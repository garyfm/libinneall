#pragma once

#include <libinneall/math/matrix4.hpp>
#include <libinneall/renderer/material.hpp>
#include <libinneall/renderer/mesh.hpp>

namespace inl {

struct Model {
    Mesh const* mesh {};
    Material const* material {};

    // TODO: Move this out to an entity class
    Matrix4 model_matrix { 1 };
};

} // namespace inl
