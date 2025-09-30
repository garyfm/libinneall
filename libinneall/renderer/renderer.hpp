#pragma once

#include <libinneall/renderer/scene.hpp>

namespace inl {

class Renderer {
public:
    Renderer() { };

    void render(Scene const& scene) const;

private:
};

} // namespace inl
