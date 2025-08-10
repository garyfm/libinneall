include_dir = '..'

source_files = files(
  'glad/src/glad.c',
  'window.cpp'
)

glfw_subproject = subproject('glfw')

glfw_dep = dependency('glfw3')
gl_dep = dependency('gl')


libinneal = static_library(
    'libinneal',
    source_files,
    include_directories: include_dir,
    dependencies: [glfw_dep, gl_dep]
)

libinneal_dep = declare_dependency(
    include_directories: include_dir,
    link_with: libinneal,
)
