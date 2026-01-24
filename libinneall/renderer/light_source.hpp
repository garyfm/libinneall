#pragma once

#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>
#include <libinneall/renderer/mesh.hpp>
#include <libinneall/renderer/shader_program.hpp>

namespace inl {

// TODO: This should probably be handled by the model
// by using different material types.
// Adding it here for quick debug rendering
struct LightSource {
    Mesh const* mesh {};
    Vector3 color {};
    ShaderProgram* shader {};

    // TODO: Move this out to an entity class
    Matrix4 model_matrix { 1 };
};

}
