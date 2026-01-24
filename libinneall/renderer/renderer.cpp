#include "renderer/light_source.hpp"
#include <libinneall/base/assert.hpp>
#include <libinneall/renderer/renderer.hpp>
#include <libinneall/renderer/shader_uniform.hpp>

namespace inl {

void Renderer::begin_frame() const {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::set_render_view(RenderView const& render_view, ShaderProgram& shader) {
    // TODO: This should be UBO which would remove the need for the shader to be passed in
    set_uniform(shader, "u_view", render_view.view);
    set_uniform(shader, "u_projection", render_view.projection);
    set_uniform(shader, "u_view_pos", render_view.pos);
}

void Renderer::render(Model const& model) {

    INL_ASSERT(model.mesh != nullptr, "Empty mesh");
    INL_ASSERT(model.material != nullptr, "Empty material");
    INL_ASSERT(model.material->shader != nullptr, "Empty shader");

    model.mesh->bind();
    model.material->shader->use();

    // TODO: texture unit is hardcoded here. Add some way to allocate these
    if (model.material->albedo != nullptr) {
        model.material->albedo->bind(0);
    }

    if (model.material->albedo != nullptr) {
        model.material->specular->bind(1);
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

void Renderer::render(LightSource const& light) {

    INL_ASSERT(light.mesh != nullptr, "Empty mesh");
    INL_ASSERT(light.shader != nullptr, "Empty shader");

    light.mesh->bind();
    light.shader->use();

    set_uniform(*light.shader, "u_color", light.color);
    set_uniform(*light.shader, "u_model", light.model_matrix);

    if (light.mesh->index_count() != 0) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(light.mesh->index_count()), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(light.mesh->vertext_count()));
    }

    light.mesh->unbind();
}

};
