#include <libinneal/renderer/gl_buffer.hpp>
#include <libinneal/renderer/mesh.hpp>
#include <libinneal/renderer/shader_program.hpp>
#include <libinneal/renderer/shader_stage.hpp>
#include <libinneal/renderer/vertex_array.hpp>
#include <libinneal/renderer/vertex_data.hpp>
#include <libinneal/utility/log.hpp>
#include <libinneal/utility/unique_resource.hpp>
#include <libinneal/window.hpp>

#include <array>
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

        log::debug("libinneal demo game");

        if (argc < 2) {
            log::error("Usage: game <resource_path>");
            return -1;
        }

        std::string resource_path = argv[1];

        Window window { 800, 600, "demo game" };

        std::string basic_vert_shader_source = read_file(resource_path + "/basic.vert.glsl");
        ShaderStage vertex_stage { ShaderType::Vertex, basic_vert_shader_source };

        std::string basic_frag_shader_source = read_file(resource_path + "/basic.frag.glsl");
        ShaderStage fragment_stage { ShaderType::Fragment, basic_frag_shader_source };

        ShaderProgram shader_program { vertex_stage, fragment_stage };
        shader_program.use();

        std::array<VertexData, 4> vertices { { { 0.5f, 0.5f, 0.0f }, { 0.5f, -0.5f, 0.0f }, { -0.5f, -0.5f, 0.0f },
            { -0.5f, 0.5f, 0.0f } } };

        std::array<unsigned, 6> indices = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
        };

        MeshData mesh_data { std::span { vertices }, std::span { indices } };
        Mesh mesh { mesh_data };

        while (!glfwWindowShouldClose(window.native_handle())) {
            window.process_input();

            // TODO: Wrap in a renderer class
            glClearColor(0.5, 0.0, 0.5, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader_program.use();

            mesh.bind();
            glDrawElements(GL_TRIANGLES, mesh.index_count(), GL_UNSIGNED_INT, 0);

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
