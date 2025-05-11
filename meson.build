project('svte', 'cpp')

src = [
        'frontend/SDL2/sdl2_events.cpp',
        'frontend/SDL2/sdl2_init.cpp',
        'frontend/SDL2/sdl2_renderer.cpp',
        'frontend/SDL2/sdl2_window.cpp',
        'frontend/SDL2/svte_sdl2_main.cpp',
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
