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

        Window window { 800, 600, "demo game" };

        std::string basic_vert_shader_source = read_file(resource_path + "/basic.vert.glsl");
        ShaderStage vertex_stage { ShaderType::Vertex, basic_vert_shader_source };
        log::debug("Created vertex shader");

        std::string basic_frag_shader_source = read_file(resource_path + "/basic.frag.glsl");
        ShaderStage fragment_stage { ShaderType::Fragment, basic_frag_shader_source };
        log::debug("Created fragment shader");

        ShaderProgram shader_program { vertex_stage, fragment_stage };
        shader_program.use();
        log::debug("Created shader program");

        std::array<VertexData, 3> vertices { {
            { -0.5f, -0.5f, 0.0f },
            { 0.0f, 0.5f, 0.0f },
            { 0.5f, -0.5f, 0.0f },
        } };

        std::array<unsigned, 3> indices = { 0, 1, 2 };

        MeshData mesh_data { std::span { vertices }, { indices } };
        Mesh mesh { mesh_data };
        Model model { &mesh, &shader_program };

        Scene scene;
        scene.models.push_back(&model);

        Renderer renderer;

        while (!glfwWindowShouldClose(window.native_handle())) {
            window.process_input();

            float varying_color = sin(glfwGetTime()) / 2.0f + 0.5f;
            Color color { 0.0f, varying_color, varying_color };
            model.shader->set_uniform("set_color", color);

            Matrix4 transform { 1 };
            transform = rotate(transform, (float)glfwGetTime(), { 0, 0, 1 });
            shader_program.set_uniform("transform", transform);

            renderer.render(scene);

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
