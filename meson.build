project('svte', 'cpp')

src = [
        'frontend/SDL2/sdl2_events.cpp',
        'frontend/SDL2/sdl2_internal.cpp',
        'frontend/SDL2/sdl2_renderer.cpp',
        'frontend/SDL2/sdl2_window.cpp',
        'frontend/SDL2/sdl2_font.cpp',
        'frontend/SDL2/svte_sdl2_main.cpp',

        'core/editor/ed_sys.cpp',
        'core/editor/ed_constructor.cpp',
        'core/editor/ed_buffers.cpp',
        'core/editor/ed_save_file.cpp',
        'core/editor/ed_open_file.cpp',

        'core/buffer/buf_constructor.cpp',
        'core/buffer/buf_remove.cpp',
        'core/buffer/buf_insert.cpp',
        'core/buffer/buf_iterator.cpp',

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
