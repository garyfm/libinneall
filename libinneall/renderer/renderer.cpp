#include <libinneall/base/assert.hpp>
#include <libinneall/base/byte.hpp>
#include <libinneall/renderer/debug_mesh.hpp>
#include <libinneall/renderer/renderer.hpp>
#include <libinneall/renderer/shader_uniform.hpp>

namespace {

// TODO: texture unit is hardcoded here. Do these need to be dynamically allocated ?
enum class TextureUnit : GLuint {
    Albedo = 0,
    Specular = 1,
    Skybox = 2,
};

}

namespace inl {

Renderer::Renderer() {
    ubo_render_view.create(sizeof(RenderView));
    glBindBufferBase(GL_UNIFORM_BUFFER, ubo_bindpoint_render_view, ubo_render_view.handle());
}

void Renderer::begin_frame() const {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    // Wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::render(RenderScene& scene, RenderView const& view) {

    for (auto& model : scene.models) {
        ShaderProgram* const shader = model.material->shader;
        shader->use();

        ubo_render_view.upload(0, to_bytes(view));

        if (scene.light_directional != nullptr) {
            set_uniform(*shader, "u_light_dir", *scene.light_directional);
        }

        set_uniform(*shader, "u_num_light_points", static_cast<int32_t>(scene.light_points.size()));
        for (size_t i = 0; i < scene.light_points.size(); ++i) {
            set_uniform(*shader, "u_light_points", scene.light_points[i], i);
        }

        if (scene.light_spot != nullptr) {
            set_uniform(*shader, "u_use_light_spot", true);
            set_uniform(*shader, "u_light_spot", *scene.light_spot);
        }

        render(model);
    }
}

void Renderer::render(Model& model) {

    INL_ASSERT(model.mesh != nullptr, "Empty mesh");
    INL_ASSERT(model.material != nullptr, "Empty material");
    INL_ASSERT(model.material->shader != nullptr, "Empty shader");

    model.mesh->bind();
    model.material->shader->use();

    if (model.material->albedo != nullptr) {
        model.material->albedo->bind(static_cast<GLuint>(TextureUnit::Albedo));
    }

    if (model.material->albedo != nullptr) {
        model.material->specular->bind(static_cast<GLuint>(TextureUnit::Specular));
    }

    set_uniform(*model.material->shader, "u_material", *model.material);

    set_uniform(*model.material->shader, "u_model", model.model_matrix);

    Matrix3 normal_matrix { 1 };
    normal_matrix = Matrix3(transpose(inverse(model.model_matrix)));
    set_uniform(*model.material->shader, "u_normal_matrix", normal_matrix);

    if (model.mesh->index_count() != 0) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.mesh->index_count()), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(model.mesh->vertext_count()));
    }
    model.mesh->unbind();
}

void Renderer::draw_debug_mesh(Mesh& mesh, const Matrix4& model_matrix, const Vector3& color) {
    INL_ASSERT(debug_shader != nullptr, "Debug shader not set");

    mesh.bind();
    debug_shader->use();
    set_uniform(*debug_shader, "u_color", color);
    set_uniform(*debug_shader, "u_model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.vertext_count()));
    mesh.unbind();
}

void Renderer::draw_debug_triangle(const Matrix4& model_matrix, const Vector3& color) {
    INL_ASSERT(debug_shader != nullptr, "Debug shader not set");

    Mesh* debug_mesh = debug_mesh_triangle();
    draw_debug_mesh(*debug_mesh, model_matrix, color);
}

void Renderer::draw_debug_quad(const Matrix4& model_matrix, const Vector3& color) {
    INL_ASSERT(debug_shader != nullptr, "Debug shader not set");

    Mesh* debug_mesh = debug_mesh_quad();
    draw_debug_mesh(*debug_mesh, model_matrix, color);
}

void Renderer::draw_debug_cube(const Matrix4& model_matrix, const Vector3& color) {
    INL_ASSERT(debug_shader != nullptr, "Debug shader not set");

    Mesh* debug_mesh = debug_mesh_cube();
    draw_debug_mesh(*debug_mesh, model_matrix, color);
}

void Renderer::draw_skybox(Cubemap& skybox) {
    INL_ASSERT(skybox_shader != nullptr, "skybox shader not set");

    glDepthMask(GL_FALSE);

    Mesh* mesh = mesh_cubemap();
    mesh->bind();

    skybox_shader->use();
    skybox.bind(static_cast<GLuint>(TextureUnit::Skybox));
    set_uniform(*skybox_shader, "skybox", skybox.unit());

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh->vertext_count()));
    mesh->unbind();

    glDepthMask(GL_TRUE);
}

}
