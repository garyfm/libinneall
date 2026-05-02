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

bool compile_shader(inl::ShaderStage const& shader_stage, inl::StringView source) {

    GLchar const* gl_source[] = { source.data() };
    GLint gl_source_length[] = { static_cast<GLint>(source.size()) };

    glShaderSource(shader_stage.handle(), 1, gl_source, gl_source_length);
    glCompileShader(shader_stage.handle());

    GLint success = 0;
    glGetShaderiv(shader_stage.handle(), GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {
        inl::String<inl::MAX_OPENGL_INFO_LOG_SIZE> info_log { inl::MAX_OPENGL_INFO_LOG_SIZE };
        GLsizei info_log_size { 0 };

        glGetShaderInfoLog(shader_stage.handle(), inl::MAX_OPENGL_INFO_LOG_SIZE, &info_log_size, info_log.data());
        info_log.resize(info_log_size);
        inl::log::error("Error compiling shader id {}: {}", shader_stage.handle(),
            inl::StringView { info_log.data(), info_log.size() });

        return false;
    }

    return true;
}
}

namespace inl {

Error ShaderStage::create(ShaderStage& shader_stage, ShaderType type, StringView source) {

    if (source.size() == 0) {
        return Error::RendererShaderStageInvalidSource;
    }

    shader_stage.m_handle.reset(glCreateShader(to_opengl(type)));
    if (!shader_stage.m_handle) {
        return Error::RendererShaderStageFailedToCreate;
    }

    shader_stage.m_type = type;

    if (!compile_shader(shader_stage, source)) {
        return Error::RendererShaderStageFailedToCompile;
    }

    log::info("Created shader stage id {}", shader_stage.m_handle.get());

    return Error::Ok;
}

} // namespace inl
