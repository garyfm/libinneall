#pragma once

#include <libinneall/renderer/scene.hpp>

namespace inl {

class Renderer {
public:
    Renderer() { glEnable(GL_DEPTH_TEST); };

    void render(Scene const& scene) const;

private:
};

} // namespace inl
