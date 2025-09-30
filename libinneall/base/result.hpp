#pragma once

#include <expected>
#include <format>
#include <string_view>

#define TRY(result)                                                                                                    \
    ({                                                                                                                 \
        if (!(result).has_value()) {                                                                                   \
            return std::unexpected((result).error());                                                                  \
        }                                                                                                              \
    })

namespace inl {

enum class Error {
    OK = 0,
    ERROR = 1,
};

template <typename T> using Result = std::expected<T, Error>;

std::string_view to_string(inl::Error e);

} // namespace inl

template <> struct std::formatter<inl::Error> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const inl::Error& error, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", inl::to_string(error));
    }
};

template <typename T> struct std::formatter<inl::Result<T>> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext> auto format(const inl::Result<T>& result, FormatContext& ctx) const {
        if (result.has_value()) {
            return std::format_to(ctx.out(), "{}", result.value());
        } else {
            return std::format_to(ctx.out(), "{}", result.error());
        }
    }
};
