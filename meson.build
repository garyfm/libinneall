project('libinneall', 'c', 'cpp', default_options: ['cpp_std=c++23'])

add_project_arguments(
  '-Werror',
  '-Wall',
  '-Wextra',
  #'-Wpedantic', # FIX: glad.c doesnt work with this
  '-Wshadow',
  '-Wformat=2',
  #'-Wconversion', # FIX: glad.c doesnt work with this
  '-Wnull-dereference',
  '-Wunreachable-code',
  '-Wsign-compare',
  '-Wfloat-equal',
  language: ['c', 'cpp']
)


subdir('libinneall')
subdir('demo')
