#pragma once

#include <libinneall/light.hpp>
#include <libinneall/math/matrix4.hpp>
#include <libinneall/math/vector3.hpp>
#include <libinneall/renderer/cubemap.hpp>
#include <libinneall/renderer/model.hpp>
#include <libinneall/renderer/shader_program.hpp>

#include <subprojects/glad/include/glad/glad.h>

#include <span>

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
    Renderer();
    void begin_frame() const;
    void render(RenderScene& scene, RenderView const& view);
    void render(Model& model);
    void set_debug_shader(ShaderProgram& shader) { debug_shader = &shader; };
    void set_skybox_shader(ShaderProgram& shader) { skybox_shader = &shader; };
    void draw_debug_mesh(Mesh& mesh, const Matrix4& model_matrix, const Vector3& color);
    void draw_debug_triangle(const Matrix4& model_matrix, const Vector3& color);
    void draw_debug_quad(const Matrix4& model_matrix, const Vector3& color);
    void draw_debug_cube(const Matrix4& model_matrix, const Vector3& color);
    void draw_skybox(Cubemap& skybox);

private:
    static constexpr uint32_t ubo_bindpoint_render_view = 0;

    ShaderProgram* debug_shader { nullptr };
    ShaderProgram* skybox_shader { nullptr };
    GlBuffer ubo_render_view { sizeof(RenderView) };
};

} // namespace inl
