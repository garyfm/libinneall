#pragma once

#include <libinneall/base/option.hpp>
#include <libinneall/base/string_view.hpp>

namespace inl {

enum class LogLevel : char {
    Debug = 'D',
    Info = 'I',
    Warn = 'W',
    Error = 'E',
};

constexpr StringView get_module_name(StringView filename) {

    const Option<size_t> slash_pos = filename.rfind("/");
    const Option<size_t> dot_pos = filename.rfind(".");

    if (!slash_pos || !dot_pos) {
        return "";
    }

    return filename.substr(*slash_pos + 1, *dot_pos - *slash_pos - 1);
}

void log(char const* filename, size_t line, LogLevel lvl, char const* fmt, ...);

} // namespace inl
  //
#define log_debug(fmt, ...) ::inl::log(__FILE__, __LINE__, ::inl::LogLevel::Info, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) ::inl::log(__FILE__, __LINE__, ::inl::LogLevel::Debug, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) ::inl::log(__FILE__, __LINE__, ::inl::LogLevel::Warn, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) ::inl::log(__FILE__, __LINE__, ::inl::LogLevel::Error, fmt, ##__VA_ARGS__)
