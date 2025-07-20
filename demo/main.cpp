#include <libinneal/log.hpp>

using namespace inl;

int main(int argc, char* argv[]) {

    log::info("Test libinneal demo game {}", 1234);
    log::debug("Test libinneal demo game {}", 1234);
    log::warn("Test libinneal demo game {}", 1234);
    log::error("Test libinneal demo game {}", 1234);

    return 0;
}
