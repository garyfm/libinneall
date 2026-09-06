#include <libinneall/asset/asset.hpp>
#include <libinneall/asset/obj.hpp>
#include <libinneall/asset/ppm.hpp>
#include <libinneall/base/arena.hpp>
#include <libinneall/base/array.hpp>
#include <libinneall/base/assert.hpp>
#include <libinneall/base/defer.hpp>
#include <libinneall/base/error.hpp>
#include <libinneall/base/log.hpp>
#include <libinneall/base/option.hpp>
#include <libinneall/base/unique_handle.hpp>
#include <libinneall/camera.hpp>
#include <libinneall/light.hpp>
#include <libinneall/math/math.hpp>
#include <libinneall/math/transforms.hpp>
#include <libinneall/mesh_data.hpp>
#include <libinneall/platform/platform.hpp>
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

#include <math.h>

namespace {

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

void process_input_keys(inl::platform::Platform& platform) {

    using namespace inl::platform;
    static constexpr float movement_speed = 4.5f;

    float velocity = movement_speed * g_delta_time;
    if (input_key_state(platform, InputKey::w) == InputKeyState::Pressed) {
        g_camera.move(inl::Camera::Direction::Forward, velocity);
    }
    if (input_key_state(platform, InputKey::s) == InputKeyState::Pressed) {
        g_camera.move(inl::Camera::Direction::Backward, velocity);
    }
    if (input_key_state(platform, InputKey::a) == InputKeyState::Pressed) {
        g_camera.move(inl::Camera::Direction::Left, velocity);
    }
    if (input_key_state(platform, InputKey::d) == InputKeyState::Pressed) {
        g_camera.move(inl::Camera::Direction::Right, velocity);
    }
}

void callback_input_mouse_pos([[maybe_unused]] inl::platform::Platform& platform, float x_pos, float y_pos) {
    static const float sensitivity { 0.05f };

    g_camera.rotate(x_pos * sensitivity, -y_pos * sensitivity);
}

[[maybe_unused]] void scroll_callback(
    [[maybe_unused]] inl::platform::Platform& platform, [[maybe_unused]] double x_offset, double y_offset) {
    g_camera.zoom(static_cast<float>(y_offset));
}

} // namespace

int main(int argc, char* argv[]) {
    using namespace inl;

    log_info("libInneall demo game");

    if (argc < 2) {
        log_error("Usage: game <assets_path>");
        return -1;
    }

    Error error {};

    platform::Platform platform {};

    error = platform::create(
        platform, "libInneal", DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, callback_input_mouse_pos);
    inl_defer(platform::destroy(platform));
    inl_assert(error == Error::Ok, "Failed to create platform");

    error = platform::window_map(platform);
    inl_assert(error == Error::Ok, "Failed to map window");

    ByteSpan scratch_backing = { allocate_backing(inl::MB * 100), inl::MB * 100 };
    inl_defer(release_backing(scratch_backing.data()));
    Arena scratch_arena { scratch_backing.data(), scratch_backing.size() };

    ByteSpan main_backing = { allocate_backing(inl::MB * 100), inl::MB * 100 };
    inl_defer(release_backing(main_backing.data()));
    Arena main_arena { main_backing.data(), main_backing.size() };

    String<MAX_ASSET_PATH_SIZE> assets_path { argv[1] };
    log_info("Asset path: %s", assets_path.data());
    size_t assets_path_root_pos { assets_path.size() };

    assets_path.append("/shaders");
    size_t shader_path_root_pos { assets_path.size() };

    ShaderProgram shader_program_lighting {};
    String<MAX_ASSET_PATH_SIZE> vert_shader_path = assets_path.append("/lighting_phong.vert.glsl");
    String<MAX_ASSET_PATH_SIZE> frag_shader_path
        = assets_path.overwrite("/lighting_phong.frag.glsl", shader_path_root_pos);
    error = load_shader(main_arena, scratch_arena, shader_program_lighting, vert_shader_path, frag_shader_path);
    inl_assert(error == Error::Ok, "Failed to load shader lighting");

    ShaderProgram shader_program_debug {};
    vert_shader_path = assets_path.overwrite("/debug.vert.glsl", shader_path_root_pos);
    frag_shader_path = assets_path.overwrite("/debug.frag.glsl", shader_path_root_pos);
    error = load_shader(main_arena, scratch_arena, shader_program_debug, vert_shader_path, frag_shader_path);
    inl_assert(error == Error::Ok, "Failed to load shader lighting");

    ShaderProgram shader_program_skybox {};
    vert_shader_path = assets_path.overwrite("/skybox.vert.glsl", shader_path_root_pos);
    frag_shader_path = assets_path.overwrite("/skybox.frag.glsl", shader_path_root_pos);
    error = load_shader(main_arena, scratch_arena, shader_program_skybox, vert_shader_path, frag_shader_path);
    inl_assert(error == Error::Ok, "Failed to load shader lighting");
    scratch_arena.reset();

    assets_path.overwrite("/backpack", assets_path_root_pos);
    size_t model_path_root_pos { assets_path.size() };

    Mesh mesh {};
    error = load_mesh(scratch_arena, mesh, assets_path.append("/mesh.obj"));
    inl_assert(error == Error::Ok, "Failed to load mesh");
    scratch_arena.reset();

    Texture texture_albedo {};
    error
        = load_texture(scratch_arena, texture_albedo, assets_path.overwrite("/albedo.ppm", model_path_root_pos), false);
    inl_assert(error == Error::Ok, "Failed to load texture_albedo");
    scratch_arena.reset();

    Texture texture_specular {};
    error = load_texture(
        scratch_arena, texture_specular, assets_path.overwrite("/specular.ppm", model_path_root_pos), false);
    inl_assert(error == Error::Ok, "Failed to load texture_albedo");
    scratch_arena.reset();

    Material material { &texture_albedo, &texture_specular, 32, &shader_program_lighting };

    Matrix4 model_matrix { 1 };

    Model model { &mesh, &material, model_matrix };

    // Skybox
    assets_path.overwrite("/skybox", assets_path_root_pos);

    Cubemap skybox {};
    error = load_cubemap(scratch_arena, skybox, assets_path, false);
    inl_assert(error == Error::Ok, "Failed to load skyboz");
    scratch_arena.reset();

    log_debug("Main arena highwater mark: %uKB", main_arena.highwater_mark() / KB);
    log_debug("Scratch arena highwater mark: %uMB", scratch_arena.highwater_mark() / MB);

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

    Renderer renderer {};
    error = Renderer::create(renderer);
    inl_assert(error == Error::Ok, "Failed to create renderer");

    renderer.set_debug_shader(shader_program_debug);
    renderer.set_skybox_shader(shader_program_skybox);

    while (!platform::window_should_exit(platform)) {
        float current_frame_time = platform::elapsed_time(platform);
        g_delta_time = current_frame_time - g_last_frame_time;
        g_last_frame_time = current_frame_time;

        render_scene.light_spot->pos = g_camera.position();
        render_scene.light_spot->dir = g_camera.front();

        RenderView render_view {
            .view = g_camera.view_matrix(),
            .projection = g_camera.perspective_matrix(aspect_ratio(platform)),
            .pos = g_camera.position(),
        };

        renderer.begin_frame();

        renderer.draw_skybox(skybox);
        renderer.render(render_scene, render_view);
        renderer.draw_debug_cube(model_matrix_light, { 1.0f, 1.0f, 1.0f });

        platform::gfx_swap_buffers(platform);
        platform::window_process_events(platform);
        process_input_keys(platform);
    }

    log_info("Exiting...");

    return 0;
}
