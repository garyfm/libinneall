#include <libinneall/base/log.hpp>
#include <libinneall/base/string.hpp>

#include <stdarg.h>
#include <stdio.h>

namespace inl {

static constexpr size_t MAX_LOG_MSG_SIZE { 1024 };
static String<MAX_LOG_MSG_SIZE> buffer {};

void log(char const* filename, size_t line, LogLevel level, char const* fmt, ...) {

    StringView module = get_module_name(filename);

    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buffer.data(), MAX_LOG_MSG_SIZE, fmt, args);
    printf("[%c][%.*s:%lu] %.*s\n", static_cast<char>(level), static_cast<int>(module.size()), module.data(), line, len,
        buffer.data());
    va_end(args);
}

} // namesspace lnl
