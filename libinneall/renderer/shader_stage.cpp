#include <libinneall/base/log.hpp>
#include <libinneall/base/string.hpp>
#include <libinneall/renderer/shader_stage.hpp>

#include <libinneall/base/string_view.hpp>

namespace {

GLuint to_opengl(inl::ShaderType type) {
    switch (type) {
    case inl::ShaderType::Vertex: {
        return GL_VERTEX_SHADER;
    }
    case inl::ShaderType::Fragment: {
        return GL_FRAGMENT_SHADER;
    }
    default:
        return 0;
    }
}
}

namespace inl {

void ShaderStage::create(ShaderType type, StringView source) {

    m_type = type;
    if (source.size() == 0) {
        throw std::runtime_error("Empty shader source");
    }

    m_handle.reset(glCreateShader(to_opengl(type)));
    if (!m_handle) {
        throw std::runtime_error("Failed to create shader");
    }

    if (!compile(source)) {
        throw std::runtime_error("Failed to complie shader");
    }

    log::info("Created shader stage id {}", m_handle.get());
};

bool ShaderStage::compile(StringView source) {

    GLchar const* gl_source[] = { source.data() };
    GLint gl_source_length[] = { static_cast<GLint>(source.size()) };

    glShaderSource(m_handle, 1, gl_source, gl_source_length);
    glCompileShader(m_handle);

    GLint success = 0;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {
        String<MAX_OPENGL_INFO_LOG_SIZE> info_log { MAX_OPENGL_INFO_LOG_SIZE };
        GLsizei info_log_size { 0 };

        glGetShaderInfoLog(m_handle, MAX_OPENGL_INFO_LOG_SIZE, &info_log_size, info_log.data());
        info_log.resize(info_log_size);
        log::error("Error compiling shader id {}: {}", m_handle.get(), StringView { info_log.data(), info_log.size() });

        return false;
    }

    return true;
}

} // namespace inl
