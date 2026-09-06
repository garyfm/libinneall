#pragma once

#include <GL/glcorearb.h>

// clang-format off
#define INL_FOR_EACH_GLFUNC(apply) \
    apply(PFNGLVIEWPORTPROC,  glViewport) \
    apply(PFNGLTEXTUREPARAMETERIPROC, glTextureParameteri) \
    apply(PFNGLATTACHSHADERPROC, glAttachShader) \
    apply(PFNGLLINKPROGRAMPROC, glLinkProgram) \
    apply(PFNGLCREATEPROGRAMPROC, glCreateProgram) \
    apply(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation) \
    apply(PFNGLGETPROGRAMIVPROC, glGetProgramiv) \
    apply(PFNGLGETINTEGERVPROC, glGetIntegerv) \
    apply(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog) \
    apply(PFNGLGETACTIVEUNIFORMPROC, glGetActiveUniform) \
    apply(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray) \
    apply(PFNGLCREATEVERTEXARRAYSPROC, glCreateVertexArrays) \
    apply(PFNGLVERTEXARRAYVERTEXBUFFERPROC, glVertexArrayVertexBuffer) \
    apply(PFNGLVERTEXARRAYELEMENTBUFFERPROC, glVertexArrayElementBuffer) \
    apply(PFNGLENABLEVERTEXARRAYATTRIBPROC, glEnableVertexArrayAttrib) \
    apply(PFNGLVERTEXARRAYATTRIBFORMATPROC, glVertexArrayAttribFormat) \
    apply(PFNGLVERTEXARRAYATTRIBBINDINGPROC, glVertexArrayAttribBinding) \
    apply(PFNGLCREATETEXTURESPROC, glCreateTextures) \
    apply(PFNGLTEXTURESUBIMAGE2DPROC, glTextureSubImage2D) \
    apply(PFNGLTEXTURESUBIMAGE3DPROC, glTextureSubImage3D) \
    apply(PFNGLBINDTEXTUREUNITPROC, glBindTextureUnit) \
    apply(PFNGLBINDBUFFERBASEPROC, glBindBufferBase) \
    apply(PFNGLTEXTURESTORAGE2DPROC, glTextureStorage2D) \
    apply(PFNGLCLEARCOLORPROC, glClearColor) \
    apply(PFNGLCLEARPROC, glClear) \
    apply(PFNGLENABLEPROC, glEnable) \
    apply(PFNGLDRAWELEMENTSPROC, glDrawElements) \
    apply(PFNGLPOLYGONMODEPROC, glPolygonMode) \
    apply(PFNGLDRAWARRAYSPROC, glDrawArrays) \
    apply(PFNGLDEPTHMASKPROC, glDepthMask) \
    apply(PFNGLGENERATETEXTUREMIPMAPPROC, glGenerateTextureMipmap) \
    apply(PFNGLSHADERSOURCEPROC, glShaderSource) \
    apply(PFNGLGETSHADERIVPROC, glGetShaderiv) \
    apply(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog) \
    apply(PFNGLCREATESHADERPROC, glCreateShader) \
    apply(PFNGLCOMPILESHADERPROC, glCompileShader) \
    apply(PFNGLCREATEBUFFERSPROC, glCreateBuffers) \
    apply(PFNGLNAMEDBUFFERSTORAGEPROC, glNamedBufferStorage) \
    apply(PFNGLNAMEDBUFFERSUBDATAPROC, glNamedBufferSubData) \
    apply(PFNGLPROGRAMUNIFORM1IPROC, glProgramUniform1i) \
    apply(PFNGLPROGRAMUNIFORM1FPROC, glProgramUniform1f) \
    apply(PFNGLPROGRAMUNIFORM3FPROC, glProgramUniform3f) \
    apply(PFNGLPROGRAMUNIFORMMATRIX3FVPROC, glProgramUniformMatrix3fv) \
    apply(PFNGLPROGRAMUNIFORMMATRIX4FVPROC, glProgramUniformMatrix4fv) \
    apply(PFNGLDELETEPROGRAMPROC, glDeleteProgram) \
    apply(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays) \
    apply(PFNGLDELETESHADERPROC, glDeleteShader) \
    apply(PFNGLDELETEBUFFERSPROC, glDeleteBuffers) \
    apply(PFNGLUSEPROGRAMPROC, glUseProgram) \
    apply(PFNGLDELETETEXTURESPROC, glDeleteTextures) \
    apply(PFNGLDEBUGMESSAGECALLBACKPROC, glDebugMessageCallback) \
    apply(PFNGLDEBUGMESSAGECONTROLPROC, glDebugMessageControl)
// clang-format on

#define GLFUNC_DECLARE(type, name) extern type name;
INL_FOR_EACH_GLFUNC(GLFUNC_DECLARE);
#undef GLFUNC_DECLARE

void gl_load_functions();
