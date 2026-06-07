#pragma once

#include <libinneall/base/string_view.hpp>

#define TRY(error)                                                                                                     \
    ({                                                                                                                 \
        auto __tmp = (error);                                                                                          \
        if ((__tmp) != inl::Error::Ok) {                                                                               \
            return __tmp;                                                                                              \
        }                                                                                                              \
        __tmp;                                                                                                         \
    })

namespace inl {

// clang-format off
#define INL_FOR_EACH_ERROR(apply) \
    apply(Ok) \
    apply(Error) \
    apply(PpmInvalidFormat) \
    apply(PpmUnsupportedFormat) \
    apply(PpmEOFReachedUnexpectedly) \
    apply(PpmFailedToExtractInteger) \
    apply(PpmInvalidWidth) \
    apply(PpmInvalidMaxValue) \
    apply(ObjInvalidFormat) \
    apply(ObjUnsupportedFormat) \
    apply(ObjEOFReachedUnexpectedly) \
    apply(ObjFailedToExtractInteger) \
    apply(ObjFailedToExtractFloat) \
    apply(ObjFaceNotTriangulated) \
    apply(AssetFailedToOpenFile) \
    apply(AssetFailedToSeekFile) \
    apply(AssetInvalidFileSize) \
    apply(AssetFailedToReadFile) \
    apply(AssetFailedToLoadFile) \
    apply(AssetFailedToLoadTexture) \
    apply(AssetFailedToLoadCubmap) \
    apply(AssetFailedToLoadShader) \
    apply(AssetFailedToLoadMesh) \
    apply(RendererFailedToCreateGlBuffer) \
    apply(RendererShaderStageInvalidSource) \
    apply(RendererShaderStageFailedToCreate) \
    apply(RendererShaderStageFailedToCompile) \
    apply(RendererShaderProgramFailedToCreate) \
    apply(RendererShaderProgramFailedToLink) \
    apply(RendererTextureFailedToCreate) \
    apply(RendererCubemapFailedToCreate) \
    apply(RendererVertexArrayFailedToCreate) \
    apply(WindowGlfwFailedToInit) \
    apply(WindowGlfwFailedToCreate) \
    apply(WindowGladFailedToLoad)

// clang-format on

#define AS_ENUM(X) X,
enum class [[nodiscard]] Error { INL_FOR_EACH_ERROR(AS_ENUM) };
#undef AS_ENUM

StringView to_string(inl::Error e);

} // namespace inl
