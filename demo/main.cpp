#include <libinneall/base/assert.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_resource.hpp>
#include <libinneall/camera.hpp>
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

// world space positions of our cubes
[[maybe_unused]] std::array<inl::Vector3, 10> cube_positions = { {
    { 0.0f,  0.0f,  0.0f},
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f},
    {2.4f, -0.4f, -3.5f},
    {-1.7f,  3.0f, -7.5f},
    { 1.3f, -2.0f, -2.5f},
    { 1.5f,  2.0f, -2.5f},
    { 1.5f,  0.2f, -1.5f},
    {-1.3f,  1.0f, -1.5f},
} };

// clang-format on

[[maybe_unused]] std::array<inl::VertexData, 3> triangle_vertices { {
    { -0.5f, -0.5f, 0.0f },
    { 0.0f, 0.5f, 0.0f },
    { 0.5f, -0.5f, 0.0f },
} };

float delta_time = 0;
float last_frame_time = 0;

inl::Camera camera { { 0.0f, 0.0f, 3.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, 2.5f };

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move(inl::Camera::Direction::Forward, delta_time);
        inl::log::info("W");
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move(inl::Camera::Direction::Backward, delta_time);
        inl::log::info("S");
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(inl::Camera::Direction::Left, delta_time);
        inl::log::info("A");
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(inl::Camera::Direction::Right, delta_time);
        inl::log::info("D");
    }
}

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

        Window window { 800, 600, "libinneall demo", &process_input };

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

        Renderer renderer;

        const Matrix4 projection_matrix { Matrix4::create_perspective(
            to_radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f) };
        shader_program.set_uniform("projection_matrix", projection_matrix);

        while (!glfwWindowShouldClose(window.native_handle())) {
            float current_frame_time = static_cast<float>(glfwGetTime());
            delta_time = current_frame_time - last_frame_time;
            last_frame_time = current_frame_time;

            window.process_input();

            shader_program.use();

            shader_program.set_uniform("view_matrix", camera.view_matrix());

            renderer.begin_frame();
            for (std::size_t i { 0 }; i < cube_positions.size(); ++i) {
                Matrix4 model_matrix { 1 };
                float angle = 20.0f * static_cast<float>(i);
                model_matrix = rotate(model_matrix, to_radians(angle), { 1.0f, 0.3f, 0.5f });
                model_matrix = translate(model_matrix, cube_positions[i]);
                shader_program.set_uniform("model_matrix", model_matrix);
                renderer.render(model);
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
