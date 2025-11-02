#pragma once

#include <libinneall/base/unique_resource.hpp>
#include <subprojects/glad/include/glad/glad.h>

#include <string_view>

namespace inl {

enum class ShaderType { Vertex, Fragment };

class ShaderStage {
public:
    ShaderStage(ShaderType type, std::string_view source);

    GLuint native_handle() const { return m_handle; }

private:
    bool compile(std::string_view source);

    static constexpr std::size_t MAX_OPENGL_INFO_LOG_SIZE = 512;

    UniqueResource<GLuint, decltype(glDeleteShader)> m_handle { 0, glDeleteShader };
    ShaderType m_type { ShaderType::Vertex };
};

} // namespace inl
