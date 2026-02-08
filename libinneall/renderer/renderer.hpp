#pragma once

#include <libinneall/light.hpp>
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

struct RenderScene {
    std::span<Model> models;
    LightDirectional* light_directional;
    std::span<LightPoint> light_points;
    LightSpot* light_spot;
};

class Renderer {
public:
    Renderer() {
        // TODO: Not sure if this should go here ?
        glEnable(GL_DEPTH_TEST);
    };

    void begin_frame() const;
    void render(RenderScene const& scene, RenderView const& view);
    void render(Model const& model);
    // TODO: Make this draw_debug_cube
    void render(LightSource const& light);

private:
    void set_render_view(RenderView const& render_view, ShaderProgram& shader);
};

} // namespace inl
