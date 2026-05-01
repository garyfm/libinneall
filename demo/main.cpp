#include <libinneall/asset/asset.hpp>
#include <libinneall/asset/obj.hpp>
#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/array.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/option.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_resource.hpp>
#include <libinneall/camera.hpp>
#include <libinneall/light.hpp>
#include <libinneall/math/math.hpp>
#include <libinneall/math/transforms.hpp>
#include <libinneall/mesh_data.hpp>
#include <libinneall/renderer/color.hpp>
#include <libinneall/renderer/cubemap.hpp>
#include <libinneall/renderer/gl_buffer.hpp>
#include <libinneall/renderer/material.hpp>
#include <libinneall/renderer/mesh.hpp>
#include <libinneall/renderer/model.hpp>
#include <libinneall/renderer/renderer.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/shader_stage.hpp>
#include <libinneall/renderer/shader_uniform.hpp>
#include <libinneall/renderer/texture.hpp>
#include <libinneall/renderer/vertex_array.hpp>
#include <libinneall/vertex_data.hpp>
#include <libinneall/window.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <chrono>
#include <cmath>

namespace {

static constexpr size_t KB = 1024;
static constexpr size_t MB = KB * 1000;
static constexpr size_t GB = MB * 1000;

static constexpr unsigned DEFAULT_SCREEN_WIDTH = 800;
static constexpr unsigned DEFAULT_SCREEN_HEIGHT = 600;

static float g_delta_time = 0;
static float g_last_frame_time = 0;

static inl::CameraInitialSettings camera_settings {
    .position = { 0.0f, 0.0f, 3.0f },
    .world_up = { 0.0f, 1.0f, 0.0f },
    .front = { 0.0f, 0.0f, -1.0f },
    .yaw = -90.0f,
    .pitch = 0.0f,
    .fov = 45.0f,
    .z_near = 0.1f,
    .z_far = 100.0f,
};

static inl::Camera g_camera(camera_settings);

void process_input(GLFWwindow* g_window);
void mouse_callback(GLFWwindow* g_window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* g_window, double x_offset, double y_offset);
void resize_callback(GLFWwindow* window, int width, int height);
inl::Window g_window { DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "libinneall demo", process_input, mouse_callback,
    scroll_callback, resize_callback };

void process_input(GLFWwindow* window) {
    // TODO: Pull this out
    static constexpr float movement_speed = 2.5f;

    float velocity = movement_speed * g_delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        g_camera.move(inl::Camera::Direction::Forward, velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        g_camera.move(inl::Camera::Direction::Backward, velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        g_camera.move(inl::Camera::Direction::Left, velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        g_camera.move(inl::Camera::Direction::Right, velocity);
    }
}

void mouse_callback([[maybe_unused]] GLFWwindow* window, double x_pos, double y_pos) {
    // TODO: Pull this out
    static const float sensitivity { 0.1f };

    static float x_prev { static_cast<float>(g_window.width()) / 2 };
    static float y_prev { static_cast<float>(g_window.width()) / 2 };
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

    g_camera.rotate(x_offset, y_offset);
}

void scroll_callback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double x_offset, double y_offset) {
    g_camera.zoom(static_cast<float>(y_offset));
}

void resize_callback([[maybe_unused]] GLFWwindow* window, int width, int height) {
    inl::log::debug("Window resized w: {} h: {}", width, height);
    g_window.resize(width, height);
}

} // namespace

// TODO: This should be an arena of some kind
static inl::Array<uint8_t, MB * 100> scratch_buffer {};

int main(int argc, char* argv[]) {
    using namespace inl;

    try {

        auto start_init = std::chrono::steady_clock::now();
        log::debug("libinneall demo game");

        if (argc < 2) {
            log::error("Usage: game <assets_path>");
            return -1;
        }

        String<MAX_ASSET_PATH_SIZE> assets_path { argv[1] };
        log::info("Asset path: {}", assets_path.data());
        size_t assets_path_root_pos { assets_path.size() };

        const std::filesystem::path model_path { assets_path.append("/backpack").data() };
        const std::filesystem::path shader_path { assets_path.overwrite("/shaders", assets_path_root_pos).data() };

        ShaderProgram shader_program_lighting {};
        Error result = load_shader(scratch_buffer, shader_program_lighting, shader_path / "lighting_phong.vert.glsl",
            shader_path / "lighting_phong.frag.glsl");
        INL_ASSERT(result == Error::Ok, "Failed to load shader lighting");

        ShaderProgram shader_program_debug {};
        result = load_shader(
            scratch_buffer, shader_program_debug, shader_path / "debug.vert.glsl", shader_path / "debug.frag.glsl");
        INL_ASSERT(result == Error::Ok, "Failed to load shader lighting");

        ShaderProgram shader_program_skybox {};
        result = load_shader(
            scratch_buffer, shader_program_skybox, shader_path / "skybox.vert.glsl", shader_path / "skybox.frag.glsl");
        INL_ASSERT(result == Error::Ok, "Failed to load shader lighting");

        Mesh mesh {};
        result = load_mesh(scratch_buffer, mesh, model_path / "mesh.obj");
        INL_ASSERT(result == Error::Ok, "Failed to load mesh");

        Texture texture_albedo {};
        result = load_texture(scratch_buffer, texture_albedo, model_path / "albedo.ppm", false);
        INL_ASSERT(result == Error::Ok, "Failed to load texture_albedo");

        Texture texture_specular {};
        result = load_texture(scratch_buffer, texture_specular, model_path / "specular.ppm", false);
        INL_ASSERT(result == Error::Ok, "Failed to load texture_albedo");

        Material material { &texture_albedo, &texture_specular, 32, &shader_program_lighting };

        Matrix4 model_matrix { 1 };

        Model model { &mesh, &material, model_matrix };

        // Skybox
        assets_path.overwrite("/skybox", assets_path_root_pos);

        Cubemap skybox {};
        result = load_cubemap(scratch_buffer, skybox, assets_path, false);
        INL_ASSERT(result == Error::Ok, "Failed to load skyboz");

        LightDirectional light_directional {
            .dir = { -0.2f, -1.0f, -0.3f },
            .ambient = { 0.01f, 0.01f, 0.01f },
            .diffuse = { 0.1f, 0.1f, 0.1f },
            .specular = { 0.2f, 0.2f, 0.2f },
        };

        LightPoint light_point {
            .pos = { 0.5f, 5.0f, 10.0f },
            .ambient = { 0.5f, 0.5f, 0.5f },
            .diffuse = { 1.0f, 1.0f, 1.0f },
            .specular = { 1.0f, 1.0f, 1.0f },
            // TODO: Use a table based on distance for these values
            .atten_constant = 1.0f,
            .atten_linear = 0.09f,
            .atten_quadratic = 0.032f,
        };

        Matrix4 model_matrix_light { 1 };
        model_matrix_light = translate(model_matrix_light, light_point.pos);
        model_matrix_light = scale(model_matrix_light, 0.1f);

        LightSpot light_spot {
            .pos = g_camera.position(),
            .dir = g_camera.front(),
            .ambient = { 0.1f, 0.1f, 0.1f },
            .diffuse = { 0.5f, 0.5f, 0.5f },
            .specular = { 1.0f, 1.0f, 1.0f },
            .inner_cutoff_cosine = cosf(to_radians(12.5f)),
            .outer_cutoff_cosine = cosf(to_radians(17.5f)),
        };

        RenderScene render_scene {
            .models = { &model, 1 },
            .light_directional = &light_directional,
            .light_points = { &light_point, 1 },
            .light_spot = &light_spot,
        };

        Renderer renderer;
        renderer.set_debug_shader(shader_program_debug);
        renderer.set_skybox_shader(shader_program_skybox);

        auto end_init = std::chrono::steady_clock::now();
        log::debug(
            "Initialisation time: {}", std::chrono::duration_cast<std::chrono::milliseconds>(end_init - start_init));

        while (!glfwWindowShouldClose(g_window.native_handle())) {
            float current_frame_time = static_cast<float>(glfwGetTime());
            g_delta_time = current_frame_time - g_last_frame_time;
            g_last_frame_time = current_frame_time;

            g_window.process_input();

            render_scene.light_spot->pos = g_camera.position();
            render_scene.light_spot->dir = g_camera.front();

            [[maybe_unused]] RenderView render_view {
                .view = g_camera.view_matrix(),
                .projection = g_camera.perspective_matrix(g_window.aspect_ratio()),
                .pos = g_camera.position(),
            };

            renderer.begin_frame();

            renderer.draw_skybox(skybox);
            renderer.render(render_scene, render_view);
            renderer.draw_debug_cube(model_matrix_light, { 1.0f, 1.0f, 1.0f });

            g_window.swap_buffers();
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
