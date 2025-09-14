#pragma once

#include <libinneal/renderer/mesh.hpp>
#include <libinneal/renderer/shader_program.hpp>

namespace inl {

struct Model {
    Mesh const* mesh;
    // TODO: Make this a material
    ShaderProgram const* shader;
};

} // namespace inl
