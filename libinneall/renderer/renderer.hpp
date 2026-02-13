#pragma once

#include <libinneall/light.hpp>
#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>
#include <libinneall/renderer/model.hpp>
#include <libinneall/renderer/shader_program.hpp>

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
    void begin_frame() const;
    void render(RenderScene const& scene, RenderView const& view);
    void render(Model const& model);
    void set_debug_shader(ShaderProgram& shader) { debug_shader = &shader; };
    void draw_debug_mesh(Mesh const& mesh, Matrix4 model_matrix, Vector3 color);
    void draw_debug_triangle(Matrix4 model_matrix, Vector3 color);
    void draw_debug_quad(Matrix4 model_matrix, Vector3 color);
    void draw_debug_cube(Matrix4 model_matrix, Vector3 color);

private:
    void set_render_view(RenderView const& render_view, ShaderProgram& shader);
    ShaderProgram* debug_shader { nullptr };
};

} // namespace inl
