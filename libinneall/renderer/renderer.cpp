#include <libinneall/renderer/gl_buffer.hpp>
#include <libinneall/renderer/renderer.hpp>

namespace inl {

void Renderer::begin_frame() const {
    glClearColor(0.0f, 0.0, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(Model const& model) const {

    model.mesh->bind();
    model.shader->use();

    if (model.mesh->index_count() != 0) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.mesh->index_count()), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(model.mesh->vertext_count()));
    }
    model.mesh->unbind();
}

};
