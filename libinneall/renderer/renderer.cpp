#include <libinneall/base/assert.hpp>
#include <libinneall/base/byte.hpp>
#include <libinneall/base/defer.hpp>
#include <libinneall/platform/gl_functions.hpp>
#include <libinneall/renderer/debug_mesh.hpp>
#include <libinneall/renderer/renderer.hpp>
#include <libinneall/renderer/shader_uniform.hpp>
#include <libinneall/renderer/texture.hpp>

namespace {

enum class TextureUnit : GLuint {
    Albedo = 0,
    Specular = 1,
    Normal = 2,
    Skybox = 3,
};

}

namespace inl {

Error Renderer::create(Renderer& renderer) {
    Error error = GlBuffer::create(renderer.m_ubo_render_view, sizeof(RenderView));

    if (error == Error::Ok) {
        glBindBufferBase(GL_UNIFORM_BUFFER, ubo_bindpoint_render_view, renderer.m_ubo_render_view.handle());
        renderer.m_created = true;
    }

    return error;
}

void Renderer::begin_frame() const {
    inl_assert(m_created, "Invalid Renderer");

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    // Wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::render(RenderScene& scene, RenderView const& view) {
    inl_assert(m_created, "Invalid Renderer");

    for (auto& model : scene.models) {
        ShaderProgram* const shader = model.material.shader;
        inl_assert(shader != nullptr, "Invalid shader");
        shader->use();

        m_ubo_render_view.upload(to_bytes(view));

        if (scene.light_directional != nullptr) {
            set_uniform(*shader, "u_light_dir", *scene.light_directional);
        }

        set_uniform(*shader, "u_num_light_points", static_cast<int32_t>(scene.light_points.size()));
        for (size_t i = 0; i < scene.light_points.size(); ++i) {
            set_uniform(*shader, "u_light_points", scene.light_points[i], i);
        }

        if (scene.light_spot != nullptr) {
            set_uniform(*shader, "u_use_light_spot", scene.light_spot->on);
            set_uniform(*shader, "u_light_spot", *scene.light_spot);
        }

        render(model);
    }
}

void Renderer::render(Model& model) {
    inl_assert(m_created, "Invalid Renderer");

    inl_assert(model.mesh != nullptr, "Empty mesh");
    inl_assert(model.material.shader != nullptr, "Empty shader");

    model.mesh->bind();
    model.material.shader->use();

    if (model.material.map_albedo) {
        model.material.map_albedo->bind(static_cast<GLuint>(TextureUnit::Albedo));
    }

    if (model.material.map_specular) {
        model.material.map_specular->bind(static_cast<GLuint>(TextureUnit::Specular));
    }
    if (model.material.map_normal) {
        model.material.map_normal->bind(static_cast<GLuint>(TextureUnit::Normal));
    }

    set_uniform(*model.material.shader, "u_material", model.material);

    set_uniform(*model.material.shader, "u_model", model.transform.matrix());

    Matrix3 normal_matrix { 1 };
    normal_matrix = Matrix3(transpose(inverse(model.transform.matrix())));
    set_uniform(*model.material.shader, "u_normal_matrix", normal_matrix);

    if (model.mesh->index_count() != 0) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.mesh->index_count()), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(model.mesh->vertext_count()));
    }

    if (model.material.map_albedo) model.material.map_albedo->unbind();
    if (model.material.map_specular) model.material.map_specular->unbind();
    if (model.material.map_normal) model.material.map_normal->unbind();

    model.mesh->unbind();
}

void Renderer::draw_debug_mesh(Mesh& mesh, const Matrix4& model_matrix, const Vector3& color) {
    inl_assert(m_created, "Invalid Renderer");
    inl_assert(m_debug_shader != nullptr, "Debug shader not set");

    mesh.bind();
    m_debug_shader->use();
    set_uniform(*m_debug_shader, "u_color", color);
    set_uniform(*m_debug_shader, "u_model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.vertext_count()));
    mesh.unbind();
}

void Renderer::draw_debug_triangle(const Matrix4& model_matrix, const Vector3& color) {
    inl_assert(m_created, "Invalid Renderer");
    inl_assert(m_debug_shader != nullptr, "Debug shader not set");

    Mesh* debug_mesh = debug_mesh_triangle();
    draw_debug_mesh(*debug_mesh, model_matrix, color);
}

void Renderer::draw_debug_quad(const Matrix4& model_matrix, const Vector3& color) {
    inl_assert(m_created, "Invalid Renderer");
    inl_assert(m_debug_shader != nullptr, "Debug shader not set");

    Mesh* debug_mesh = debug_mesh_quad();
    draw_debug_mesh(*debug_mesh, model_matrix, color);
}

void Renderer::draw_debug_cube(const Matrix4& model_matrix, const Vector3& color) {
    inl_assert(m_created, "Invalid Renderer");
    inl_assert(m_debug_shader != nullptr, "Debug shader not set");

    Mesh* debug_mesh = debug_mesh_cube();
    draw_debug_mesh(*debug_mesh, model_matrix, color);
}

void Renderer::draw_debug_light_point(inl::LightPoint& point) {
    inl::Matrix4 model_matrix_light { 1 };
    model_matrix_light = scale(model_matrix_light, 0.1f);
    model_matrix_light = translate(model_matrix_light, point.pos);
    draw_debug_cube(model_matrix_light, { 1.0f, 1.0f, 1.0f });
}

void Renderer::draw_debug_textured_quad(const Matrix4& model_matrix, Texture& texture) {
    inl_assert(m_created, "Invalid Renderer");
    inl_assert(m_debug_shader != nullptr, "Debug shader not set");

    Mesh* debug_mesh = debug_mesh_quad();

    m_debug_shader->use();
    texture.bind(static_cast<GLuint>(TextureUnit::Albedo));
    set_uniform(*m_debug_shader, "u_map_albedo", texture.unit());

    set_uniform(*m_debug_shader, "u_use_texture", true);
    draw_debug_mesh(*debug_mesh, model_matrix, {});
    set_uniform(*m_debug_shader, "u_use_texture", false);

    texture.unbind();
}

void Renderer::draw_skybox(Cubemap& skybox) {
    inl_assert(m_created, "Invalid Renderer");
    inl_assert(m_skybox_shader != nullptr, "skybox shader not set");

    glDepthMask(GL_FALSE);

    Mesh* mesh = mesh_cubemap();
    mesh->bind();

    m_skybox_shader->use();
    skybox.bind(static_cast<GLuint>(TextureUnit::Skybox));
    set_uniform(*m_skybox_shader, "skybox", skybox.unit());

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh->vertext_count()));
    mesh->unbind();

    glDepthMask(GL_TRUE);
}

}
