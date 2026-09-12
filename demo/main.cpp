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

    ByteSpan scratch_backing = { static_cast<uint8_t*>(platform::mem_alloc(inl::MB * 100)), inl::MB * 100 };
    inl_defer(platform::mem_free(scratch_backing.data(), scratch_backing.size()));
    Arena scratch_arena { scratch_backing.data(), scratch_backing.size() };

    ByteSpan main_backing = { static_cast<uint8_t*>(platform::mem_alloc(inl::MB * 100)), inl::MB * 100 };
    inl_defer(platform::mem_free(main_backing.data(), main_backing.size()));
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

    AssetStore asset_store {};

    assets_path.overwrite("/backpack", assets_path_root_pos);

    Model* model_backpack = asset_store.push_model();
    error = load_model(scratch_arena, asset_store, *model_backpack, assets_path);
    inl_assert(error == Error::Ok, "Failed to load model");
    scratch_arena.reset();
    model_backpack->material.shader = &shader_program_lighting;

    assets_path.overwrite("/sword-sting", assets_path_root_pos);
    Model* model_sword = asset_store.push_model();
    error = load_model(scratch_arena, asset_store, *model_sword, assets_path);
    inl_assert(error == Error::Ok, "Failed to load model");
    scratch_arena.reset();
    model_sword->material.shader = &shader_program_lighting;

    model_sword->model_matrix = scale(model_sword->model_matrix, 0.06f);
    model_sword->model_matrix = rotate(model_sword->model_matrix, to_radians(-90), { 1, 0, 0 });
    model_sword->model_matrix = translate(model_sword->model_matrix, { 5, 0, 0 });

    assets_path.overwrite("/woodhouse", assets_path_root_pos);
    Model* model_house = asset_store.push_model();
    error = load_model(scratch_arena, asset_store, *model_house, assets_path);
    inl_assert(error == Error::Ok, "Failed to load model");
    scratch_arena.reset();
    model_house->model_matrix = scale(model_house->model_matrix, 5.00f);
    model_house->model_matrix = translate(model_house->model_matrix, { -20, -2, 0 });
    model_house->material.shader = &shader_program_lighting;

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
    model_matrix_light = scale(model_matrix_light, 0.1f);
    model_matrix_light = translate(model_matrix_light, light_point.pos);

    LightSpot light_spot {
        .pos = g_camera.position(),
        .dir = g_camera.front(),
        .ambient = { 0.1f, 0.1f, 0.1f },
        .diffuse = { 0.5f, 0.5f, 0.5f },
        .specular = { 1.0f, 1.0f, 1.0f },
        .inner_cutoff_cosine = cosf(to_radians(12.5f)),
        .outer_cutoff_cosine = cosf(to_radians(17.5f)),
    };

    Matrix4 model_matrix_ground { 1 };
    // TODO: fix transforms
    model_matrix_ground = scale(model_matrix_ground, 100.0f);
    model_matrix_ground = rotate(model_matrix_ground, to_radians(-90.0f), { 1, 0, 0 });
    model_matrix_ground = translate(model_matrix_ground, { 0, -2, 0 });

    RenderScene render_scene {
        .models = asset_store.models(),
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

        // Ground
        renderer.draw_debug_quad(model_matrix_ground, { 0.2f, 0.2f, 0.2f });

        platform::gfx_swap_buffers(platform);
        platform::window_process_events(platform);
        process_input_keys(platform);
    }

    return 0;
}
