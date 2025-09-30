#pragma once

#include <libinneall/renderer/mesh.hpp>
#include <libinneall/renderer/shader_program.hpp>

namespace inl {

struct Model {
    Mesh const* mesh { nullptr };
    // TODO: Make this a material
    ShaderProgram const* shader { nullptr };
};

} // namespace inl
