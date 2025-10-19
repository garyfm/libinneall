include_dir = '..'

source_files = files(
  'base/assert.cpp',
  'base/result.cpp',
  'glad/src/glad.c',
  'math/matrix3.cpp',
  'math/matrix4.cpp',
  'math/vector2.cpp',
  'math/vector3.cpp',
  'math/vector4.cpp',
  'renderer/gl_buffer.cpp',
  'renderer/mesh.cpp',
  'renderer/renderer.cpp',
  'renderer/shader_program.cpp',
  'renderer/shader_stage.cpp',
  'renderer/vertex_array.cpp',
  'renderer/vertex_data.cpp',
  'window.cpp',
)

test_files = files(
    'doctest.cpp',
    'math/matrix3.test.cpp',
    'math/matrix4.test.cpp',
    'math/vector2.test.cpp',
    'math/vector3.test.cpp',
    'math/vector4.test.cpp',
)

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

libinneal_tests = executable('libinneal_tests',
    test_files,
    link_with: libinneall,
    dependencies: [doctest_dep, libinneall_dep]
)
test('libinneal', libinneal_tests)

