#include <libinneal/log.hpp>
#include <libinneal/utility/unique_resource.hpp>
#include <libinneal/window.hpp>

using namespace inl;

void deleter(unsigned) { log::info("Deleter called"); }

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {

    try {

        log::debug("libinneal demo game");

        Window window { 800, 600, "demo game" };

        while (!glfwWindowShouldClose(window.native_handle())) {
            window.process_input();
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
