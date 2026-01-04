#include <libinneall/renderer/gl_buffer.hpp>
#include <libinneall/renderer/renderer.hpp>
#include <libinneall/renderer/shader_uniform.hpp>

namespace inl {

void Renderer::begin_frame() const {
    glClearColor(0.0f, 0.0, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(Model const& model) const {

    model.mesh->bind();
    model.material->shader->use();

    // TODO: texture unit is hardcoded here. Add some way to allocate these
    model.material->albedo->bind(0);
    model.material->specular->bind(1);

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

};
