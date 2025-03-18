project('SVTE', 'cpp', default_options: ['bindir=bin'])
src = [
        'src/svte.cpp',
        'src/audio.cpp',
        'src/editor.cpp',
        'src/buffer.cpp',
        'src/font.cpp',
        'src/render_draw.cpp',
        'src/chars.cpp',
        'src/renderer.cpp',
        'src/window.cpp',
]

sdl2_dep = dependency('SDL2')
sdl2_ttf_dep = dependency('SDL2_ttf')
cmp_time_args = [
    '-Wall', '-Wextra', '-Wpedantic'
]

target = executable(
        meson.project_name(),
        src,
        dependencies: [sdl2_dep, sdl2_ttf_dep],
        c_args: cmp_time_args
)
