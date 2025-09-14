#include <libinneal/renderer/renderer.hpp>

namespace inl {
void Renderer::render(Scene const& scene) const {

    glClearColor(0.5, 0.0, 0.5, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (const auto model : scene.models) {
        model->mesh->bind();
        model->shader->use();
        glDrawElements(GL_TRIANGLES, model->mesh->index_count(), GL_UNSIGNED_INT, 0);
    }
}
};
