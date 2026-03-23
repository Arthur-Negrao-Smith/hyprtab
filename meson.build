project(
  'HyprTab',
  'cpp',
  version: '0.1.0',
  default_options: ['warning_level=3', 'cpp_std=c++17']
)

include_dir = include_directories('src/include')

src = [
  'src/main.cpp',
  'src/ipc.cpp',
  'src/window.cpp',
  'src/view.cpp'
]

gtkmm_dep = dependency('gtkmm-4.0')

gtk4_layer_shell_dep = dependency('gtk4-layer-shell-0')

executable('hyprtab',
  src,
  dependencies: [
    gtkmm_dep,
    gtk4_layer_shell_dep
  ],
  include_directories: include_dir,
  install: true
)
