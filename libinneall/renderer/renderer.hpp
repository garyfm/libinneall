#pragma once

#include <libinneall/renderer/scene.hpp>

namespace inl {

class Renderer {
public:
    Renderer() {
        // TODO: Not sure if this should go here ?
        glEnable(GL_DEPTH_TEST);
    };

    void begin_frame() const;
    void render(Model const& model) const;

private:
};

} // namespace inl
