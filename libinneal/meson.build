include_dir = '..'
libinneal = static_library(
    'libinneal',
    'log.cpp',
    include_directories: include_dir,
)

libinneal_dep = declare_dependency(
    include_directories: include_dir,
    link_with: libinneal,
)
