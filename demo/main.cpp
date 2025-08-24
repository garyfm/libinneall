#include <libinneal/log.hpp>
#include <libinneal/renderer/shader_program.hpp>
#include <libinneal/renderer/shader_stage.hpp>
#include <libinneal/utility/unique_resource.hpp>
#include <libinneal/window.hpp>

#include <filesystem>
#include <fstream>

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
        ShaderStage vertex_stage(ShaderType::Vertex, basic_vert_shader_source);

        std::string basic_frag_shader_source = read_file(resource_path + "/basic.frag.glsl");
        ShaderStage fragment_stage(ShaderType::Fragment, basic_frag_shader_source);

        ShaderProgram shader_program(vertex_stage, fragment_stage);
        shader_program.use();

        std::array<float, 9> vertices { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f };

        auto delete_vertex_buffer = [](GLuint buffer) { glDeleteBuffers(1, &buffer); };
        UniqueResource vertex_buffer { GLuint {}, delete_vertex_buffer };

        auto delete_vertex_array = [](GLuint array) { glDeleteVertexArrays(1, &array); };
        UniqueResource vertex_array { GLuint {}, delete_vertex_array };

        {
            glGenVertexArrays(1, &vertex_array);
            glGenBuffers(1, &vertex_buffer);

            glBindVertexArray(vertex_array);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        while (!glfwWindowShouldClose(window.native_handle())) {

            window.process_input();

            glClearColor(0.5, 0.0, 0.5, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader_program.use();
            glBindVertexArray(vertex_array);
            glDrawArrays(GL_TRIANGLES, 0, 3);

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
