#pragma once

namespace inl {

class Texture;
class ShaderProgram;

struct Material {
    Texture* albedo {};
    Texture* specular {};
    float shininess {};

    ShaderProgram* shader {};
};

} // namespace inl
