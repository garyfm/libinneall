#pragma once

#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>
#include <libinneall/renderer/light_source.hpp>
#include <libinneall/renderer/model.hpp>

#include <subprojects/glad/include/glad/glad.h>

namespace inl {

struct RenderView {
    Matrix4 view;
    Matrix4 projection;
    Vector3 pos;
};

class Renderer {
public:
    Renderer() {
        // TODO: Not sure if this should go here ?
        glEnable(GL_DEPTH_TEST);
    };

    void begin_frame() const;
    void set_render_view(RenderView const& render_view, ShaderProgram& shader);
    void render(Model const& model);
    void render(LightSource const& light);

private:
};

} // namespace inl
