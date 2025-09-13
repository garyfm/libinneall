#pragma once

#include <format>
#include <iterator>
#include <print>
#include <source_location>
#include <string_view>

namespace inl::log {

enum class Level : char {
    Debug = 'd',
    Info = 'i',
    Warn = 'w',
    Error = 'e',
};

void log_out(Level level, std::string_view module, std::string_view msg);

constexpr std::string_view get_module_name(const std::source_location location) {
    std::string_view file_name { location.file_name() };

    const std::size_t slash_pos = file_name.find_last_of("/\\");
    const std::size_t dot_pos = file_name.find_last_of("\\.");

    return file_name.substr(slash_pos + 1, dot_pos - slash_pos - 1);
}

// Use template deduction guide to get arround variadic args and defaulted args
template <Level L, typename... Args>
struct Log {
    Log(std::format_string<Args...> fmt, Args&&... args,
        const std::source_location location = std::source_location::current()) {
        char level = '?';

        if constexpr (L == Level::Debug) {
            level = 'D';
        } else if constexpr (L == Level::Info) {
            level = 'I';
        } else if constexpr (L == Level::Warn) {
            level = 'W';
        } else if constexpr (L == Level::Error) {
            level = 'E';
        }

        std::println("[{}][{}:{}] {}", level, get_module_name(location), location.line(),
            std::format(fmt, std::forward<Args>(args)...));
    }
};

template <Level L = {}, typename... Args>
Log(std::string_view fmt, Args&&...) -> Log<L, Args...>;

template <typename... Args>
using debug = Log<Level::Debug, Args...>;
template <typename... Args>
using info = Log<Level::Info, Args...>;
template <typename... Args>
using warn = Log<Level::Warn, Args...>;
template <typename... Args>
using error = Log<Level::Error, Args...>;
} // namespace inl::log
