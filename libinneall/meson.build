include_dir = '..'

source_files = files(
  'glad/src/glad.c',
  'base/assert.cpp',
  'base/result.cpp',
  'window.cpp',
  'renderer/gl_buffer.cpp',
  'renderer/mesh.cpp',
  'renderer/renderer.cpp',
  'renderer/shader_program.cpp',
  'renderer/shader_stage.cpp',
  'renderer/vertex_array.cpp',
  'renderer/vertex_data.cpp',
)

glfw_subproject = subproject('glfw')

glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')
doctest_dep = dependency('doctest')

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

# TODO: Add a way to auto generate this for all tests 
vector3_test = executable('vector3_test',
    'math/vector3.test.cpp',
    link_with: libinneall,
    dependencies: [doctest_dep, libinneall_dep]
)

test('vector3', vector3_test)

