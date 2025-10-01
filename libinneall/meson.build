include_dir = '..'

source_files = files(
  'base/assert.cpp',
  'base/result.cpp',
  'glad/src/glad.c',
  'math/vector3.cpp',
  'renderer/gl_buffer.cpp',
  'renderer/mesh.cpp',
  'renderer/renderer.cpp',
  'renderer/shader_program.cpp',
  'renderer/shader_stage.cpp',
  'renderer/vertex_array.cpp',
  'renderer/vertex_data.cpp',
  'window.cpp',
)

glfw_subproject = subproject('glfw')

glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')

libinneall = static_library(
    'libinneall',
    source_files,
    include_directories: include_dir,
    dependencies: [glfw_dep, gl_dep]
)

libinneall_dep = declare_dependency(
    include_directories: include_dir,
    link_with: libinneall,
)
