project('libinneall', 'c', 'cpp', 
  default_options: ['cpp_std=c++23']
)

add_project_arguments(
  '-Werror',
  '-Wall',
  '-Wextra',
  '-Wshadow',
  '-Wformat=2',
  '-Wconversion', 
  '-Wnull-dereference',
  '-Wunreachable-code',
  '-Wsign-compare',
  '-Wfloat-equal',
  '-fno-exceptions',
  '-fno-rtti',
  language: ['c', 'cpp']
)

if get_option('debug')
  add_project_arguments('-DBUILD_DEBUG=1', language: 'cpp')
endif

subdir('libinneall')
subdir('demo')
