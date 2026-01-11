#include <libinneall/asset/obj.hpp>
#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/result.hpp>
#include <libinneall/base/unique_resource.hpp>
#include <libinneall/camera.hpp>
#include <libinneall/light.hpp>
#include <libinneall/math/math.hpp>
#include <libinneall/math/transforms.hpp>
#include <libinneall/mesh_data.hpp>
#include <libinneall/renderer/color.hpp>
#include <libinneall/renderer/gl_buffer.hpp>
#include <libinneall/renderer/material.hpp>
#include <libinneall/renderer/mesh.hpp>
#include <libinneall/renderer/model.hpp>
#include <libinneall/renderer/renderer.hpp>
#include <libinneall/renderer/scene.hpp>
#include <libinneall/renderer/shader_program.hpp>
#include <libinneall/renderer/shader_stage.hpp>
#include <libinneall/renderer/shader_uniform.hpp>
#include <libinneall/renderer/texture.hpp>
#include <libinneall/renderer/vertex_array.hpp>
#include <libinneall/vertex_data.hpp>
#include <libinneall/window.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <array>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <span>

namespace {

static constexpr unsigned DEFAULT_SCREEN_WIDTH = 800;
static constexpr unsigned DEFAULT_SCREEN_HEIGHT = 600;

// TODO: Implement these with proper error handling
std::string read_file(std::filesystem::path path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        inl::log::error("Failed to open file: {}", path.c_str());
    }

    std::size_t file_size = std::filesystem::file_size(path);
    std::string contents(file_size, '\0');

    file.read(contents.data(), file_size);

    return contents;
}

void read_file(std::filesystem::path path, std::vector<std::uint8_t>& buffer) {
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        inl::log::error("Failed to open file: {}", path.c_str());
    }

    std::size_t file_size = std::filesystem::file_size(path);
    buffer.resize(file_size);
    file.read(reinterpret_cast<char*>(buffer.data()), file_size);
}

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

std::optional<inl::Texture> load_texture(std::filesystem::path path, bool flip_vertically) {

    using namespace inl;

    std::vector<std::uint8_t> raw_image_data {};
    auto start_image = std::chrono::steady_clock::now();
    read_file(path, raw_image_data);
    log::debug("Image size: {}", raw_image_data.size());

    ppm::Result<ppm::Image> image_or_error = ppm::load(raw_image_data);
    if (!image_or_error) {
        log::error("Failed to load ppm image error: {}", static_cast<int>(image_or_error.error()));
        return std::nullopt;
    }

    auto end_image = std::chrono::steady_clock::now();
    log::debug("PPM image: f:{}, w:{}, h:{}, v:{}, size:{}, time:{} ", static_cast<int>(image_or_error->format),
        image_or_error->width, image_or_error->height, image_or_error->max_value, image_or_error->pixel_data.size(),
        std::chrono::duration_cast<std::chrono::milliseconds>(end_image - start_image));

    ppm::Image image;
    if (flip_vertically) {
        image = ppm::flip_vertically(*image_or_error);
    } else {
        image = *image_or_error;
    }

    Texture texture { image.width, image.height, 3, image.pixel_data.data() };

    glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture.native_handle(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

// static const std::string obj_file = "/sword-sting/sting-sword.obj";
// static const std::string texture_albedo_file = "/sword-sting/Sting_Base_Color.ppm";
// static const std::string texture_specular_file = "/sword-sting/Sting_Metallic.ppm";
// bool flip_image = true;

static const std::string obj_file = "/backpack/backpack.obj";
static const std::string texture_albedo_file = "/backpack/diffuse.ppm";
static const std::string texture_specular_file = "/backpack/specular.ppm";
bool flip_image = false;
}

int main(int argc, char* argv[]) {
    using namespace inl;

    try {

        auto start_init = std::chrono::steady_clock::now();
        log::debug("libinneall demo game");

        if (argc < 2) {
            log::error("Usage: game <resource_path>");
            return -1;
        }

        std::string resource_path = argv[1];

        log::debug("Creating vertex shader");
        std::string basic_vert_shader_source = read_file(resource_path + "/shaders/basic_lighting.vert.glsl");
        ShaderStage vertex_stage { ShaderType::Vertex, basic_vert_shader_source };

        log::debug("Creating fragment shader");
        std::string basic_frag_shader_source = read_file(resource_path + "/shaders/basic_lighting.frag.glsl");
        ShaderStage fragment_stage { ShaderType::Fragment, basic_frag_shader_source };

        log::debug("Creating shader program");
        ShaderProgram shader_program { vertex_stage, fragment_stage };

        auto start_load = std::chrono::steady_clock::now();
        std::string obj_data = read_file(resource_path + obj_file);

        obj::Result<obj::Model> obj_model = obj::load(obj_data);
        auto end_load = std::chrono::steady_clock::now();
        if (!obj_model) {
            log::error("Failed to load obj file error: {}", static_cast<int>(obj_model.error()));
            return -1;
        }

        log::debug("OBJ model: vertices:{}, textures:{}, normals:{}, faces:{}, time:{}",
            obj_model->geometric_vertices.size(), obj_model->texture_vertices.size(), obj_model->vertex_normals.size(),
            obj_model->face_corners.size(),
            std::chrono::duration_cast<std::chrono::milliseconds>(end_load - start_load));

        auto start_mesh = std::chrono::steady_clock::now();
        MeshData mesh_data = to_mesh_data(*obj_model);
        auto end_mesh = std::chrono::steady_clock::now();
        log::debug("MeshData: vertices:{}, indices:{}, time:{}", mesh_data.vertex_data.size(),
            mesh_data.index_data.size(), std::chrono::duration_cast<std::chrono::milliseconds>(end_mesh - start_mesh));

        Mesh mesh { mesh_data };

        std::optional<Texture> texture_albedo { load_texture(resource_path + texture_albedo_file, flip_image) };
        INL_ASSERT(texture_albedo.has_value(), "Failed to load texture_albedo");

        std::optional<Texture> texture_specular { load_texture(resource_path + texture_specular_file, flip_image) };
        INL_ASSERT(texture_albedo.has_value(), "Failed to load texture_albedo");

        Material material { &texture_albedo.value(), &texture_specular.value(), 32, &shader_program };

        Matrix4 model_matrix { 1 };

        Model model { &mesh, &material, model_matrix };

        Light light {
            .pos = { 1.2f, 1.0f, 2.0f },
            .ambient = { 0.5f, 0.5f, 0.5f },
            .diffuse = { 0.8f, 0.8f, 0.8f },
            .specular = { 1.0f, 1.0f, 1.0f },
        };

        auto end_init = std::chrono::steady_clock::now();

        Renderer renderer;

        log::debug(
            "Initialisation time: {}", std::chrono::duration_cast<std::chrono::milliseconds>(end_init - start_init));
        while (!glfwWindowShouldClose(g_window.native_handle())) {
            float current_frame_time = static_cast<float>(glfwGetTime());
            g_delta_time = current_frame_time - g_last_frame_time;
            g_last_frame_time = current_frame_time;

            g_window.process_input();

            set_uniform(*model.material->shader, "u_light", light);

            RenderView render_view {
                .view = g_camera.view_matrix(),
                .projection = g_camera.perspective_matrix(g_window.aspect_ratio()),
                .pos = g_camera.position(),
            };

            renderer.begin_frame();

            renderer.set_render_view(render_view, *model.material->shader);
            renderer.render(model);

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
