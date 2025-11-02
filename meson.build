project('libinneall', 'c', 'cpp', default_options: ['cpp_std=c++23'])

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
  language: ['c', 'cpp']
)


subdir('libinneall')
subdir('demo')
