#include <libinneall/base/assert.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_resource.hpp>
#include <libinneall/math/math.hpp>
#include <libinneall/math/transforms.hpp>
#include <libinneall/renderer/color.hpp>
#include <libinneall/renderer/gl_buffer.hpp>
#include <libinneall/renderer/mesh.hpp>
#include <libinneall/renderer/model.hpp>
#include <libinneall/renderer/renderer.hpp>
#include <libinneall/renderer/scene.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/shader_stage.hpp>
#include <libinneall/renderer/vertex_array.hpp>
#include <libinneall/renderer/vertex_data.hpp>
#include <libinneall/window.hpp>

#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <span>

namespace {

std::string read_file(std::filesystem::path path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    std::size_t file_size = std::filesystem::file_size(path);

    std::string contents(file_size, '\0');

    file.read(contents.data(), file_size);

    return contents;
}

// clang-format off
std::array<inl::VertexData, 36> cube_vertices { {
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},

    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},

    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},

    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},

    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f, -0.5f},

    {-0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},
} };
// clang-format on

[[maybe_unused]] std::array<inl::VertexData, 3> trigangle_vertices { {
    { -0.5f, -0.5f, 0.0f },
    { 0.0f, 0.5f, 0.0f },
    { 0.5f, -0.5f, 0.0f },
} };

}

int main(int argc, char* argv[]) {
    using namespace inl;

    try {

        log::debug("libinneall demo game");

        if (argc < 2) {
            log::error("Usage: game <resource_path>");
            return -1;
        }

        std::string resource_path = argv[1];

        Window window { 800, 600, "libinneall demo" };

        log::debug("Creating vertex shader");
        std::string basic_vert_shader_source = read_file(resource_path + "/basic.vert.glsl");
        ShaderStage vertex_stage { ShaderType::Vertex, basic_vert_shader_source };

        log::debug("Creating fragment shader");
        std::string basic_frag_shader_source = read_file(resource_path + "/basic.frag.glsl");
        ShaderStage fragment_stage { ShaderType::Fragment, basic_frag_shader_source };

        log::debug("Creating shader program");
        ShaderProgram shader_program { vertex_stage, fragment_stage };
        shader_program.use();

        MeshData mesh_data { std::span { cube_vertices }, {} };
        Mesh mesh { mesh_data };
        Model model { &mesh, &shader_program };

        Scene scene;
        scene.models.push_back(&model);

        Renderer renderer;

        while (!glfwWindowShouldClose(window.native_handle())) {
            window.process_input();

            shader_program.use();

            // Camera
            {
                const float radius = 10.0f;

                float camera_x = sinf(static_cast<float>(glfwGetTime())) * radius;
                float camera_z = cosf(static_cast<float>(glfwGetTime())) * radius;

                const Vector3 camera_position { camera_x, 0.0f, camera_z };
                const Vector3 camera_target { 0.0f, 0.0f, 0.0f };
                const Vector3 world_up { 0.0f, 1.0f, 0.0f };

                const Matrix4 view_matrix { Matrix4::create_look_at(camera_position, camera_target, world_up) };
                shader_program.set_uniform("view_matrix", view_matrix);
            }

            Matrix4 projection_matrix { Matrix4::create_perspective(to_radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f) };
            shader_program.set_uniform("projection_matrix", projection_matrix);

            Matrix4 model_matrix { 1 };
            shader_program.set_uniform("model_matrix", model_matrix);

            renderer.begin_frame();
            for (auto const m : scene.models) {
                renderer.render(*m);
            }

            window.swap_buffers();
        }
    } catch (const std::exception& e) {
        log::error("Exception: {}", e.what());
        return -1;
    } catch (...) {
        log::error("Unknown Exception");
        return -1;
    }

    return 0;
}
