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

static constexpr unsigned SCREEN_WIDTH = 800;
static constexpr unsigned SCREEN_HEIGHT = 600;
static constexpr float ASPECT_RATIO = SCREEN_WIDTH / SCREEN_HEIGHT;

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

inl::Camera camera { { 0.0f, 0.0f, 3.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, -90.0f, 0.0f };

void process_input(GLFWwindow* window) {
    // TODO: Pull this out
    static constexpr float movement_speed = 2.5f;

    float velocity = movement_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move(inl::Camera::Direction::Forward, velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move(inl::Camera::Direction::Backward, velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(inl::Camera::Direction::Left, velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(inl::Camera::Direction::Right, velocity);
    }
}

void mouse_callback([[maybe_unused]] GLFWwindow* window, double x_pos, double y_pos) {
    // TODO: Pull this out
    static const float sensitivity { 0.1f };

    static float x_prev { SCREEN_WIDTH / 2 };
    static float y_prev { SCREEN_HEIGHT / 2 };
    static bool first_mouse_movement { false };

    const float x_pos_f = static_cast<float>(x_pos);
    const float y_pos_f = static_cast<float>(y_pos);

    if (first_mouse_movement) {
        first_mouse_movement = false;

        x_prev = x_pos_f;
        y_prev = y_pos_f;
    }

    const float x_offset = (x_pos_f - x_prev) * sensitivity;
    const float y_offset = (y_prev - y_pos_f) * sensitivity; // NOTE: Reversed as y goes from bottom to top

    x_prev = static_cast<float>(x_pos);
    y_prev = static_cast<float>(y_pos);

    camera.rotate(x_offset, y_offset);
}

static float g_fov { 45.0f };

void scroll_callback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double x_offset, double y_offset) {
    g_fov -= static_cast<float>(y_offset);
    g_fov = inl::clamp(g_fov, 1.0f, 45.0f);
    inl::log::info("FOV: {}", g_fov);
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

        Window window { SCREEN_WIDTH, SCREEN_HEIGHT, "libinneall demo", process_input, mouse_callback,
            scroll_callback };

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

        while (!glfwWindowShouldClose(window.native_handle())) {
            float current_frame_time = static_cast<float>(glfwGetTime());
            delta_time = current_frame_time - last_frame_time;
            last_frame_time = current_frame_time;

            window.process_input();

            shader_program.use();

            const Matrix4 projection_matrix { Matrix4::create_perspective(
                to_radians(g_fov), ASPECT_RATIO, 0.1f, 100.0f) };
            shader_program.set_uniform("projection_matrix", projection_matrix);

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
