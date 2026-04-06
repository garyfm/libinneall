#pragma once

#include <format>
#include <libinneall/base/string_view.hpp>
#include <print>
#include <source_location>

namespace inl::log {

enum class Level : char {
    Debug = 'd',
    Info = 'i',
    Warn = 'w',
    Error = 'e',
};

constexpr StringView get_module_name(const std::source_location& location) {
    StringView file_name { location.file_name() };

    const std::optional<size_t> slash_pos = file_name.rfind("/");
    const std::optional<size_t> dot_pos = file_name.rfind(".");

    if (!slash_pos || !dot_pos) {
        return "";
    }

    return file_name.substr(*slash_pos + 1, *dot_pos - *slash_pos - 1);
}

// Use template deduction guide to get arround variadic args and defaulted args
template <Level L, typename... Args> struct Log {
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

        StringView module = get_module_name(location);
        std::println("[{}][{:.{}}:{}] {}", level, module.data(), module.size(), location.line(),
            std::format(fmt, std::forward<Args>(args)...));
    }
};

template <Level L = {}, typename... Args> Log(StringView fmt, Args&&...) -> Log<L, Args...>;

template <typename... Args> using debug = Log<Level::Debug, Args...>;
template <typename... Args> using info = Log<Level::Info, Args...>;
template <typename... Args> using warn = Log<Level::Warn, Args...>;
template <typename... Args> using error = Log<Level::Error, Args...>;
} // namespace inl::log
