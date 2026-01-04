#pragma once

#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/texture.hpp>

namespace inl {

struct Material {
    Texture* albedo {};
    Texture* specular {};
    float shininess {};

    ShaderProgram* shader {};
};

} // namespace inl
