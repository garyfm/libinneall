#include <libinneal/renderer/vertex_data.hpp>

#include <span>

namespace inl {

std::span<std::byte const> as_bytes(VertexData const* const vertex_data, std::size_t count) {
    return std::as_bytes(std::span<VertexData const>(vertex_data, count));
}

} // namespace
