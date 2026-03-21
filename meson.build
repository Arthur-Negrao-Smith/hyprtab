project(
  'HyprTab',
  'cpp',
  version: '0.1.0',
  default_options: ['warning_level=3', 'cpp_std=c++17']
)

include_dir = include_directories('src/include')

src = [
  'src/main.cpp', 
  'src/ipc.cpp'
]

executable('hyprtab',
  src,
  include_directories: include_dir,
  install: true
)
