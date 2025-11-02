project('glad', 'c')

glad = static_library(
    'glad',
    'src/glad.c',
    include_directories: 'include',
)

glad_dep = declare_dependency(
    include_directories: 'include',
    link_with: glad,
)
