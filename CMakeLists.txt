
cmake_minimum_required(VERSION 3.16)
project(svte VERSION 1.0)

set(CMAKE_CPP_STANDARD 17)
set(CMAKE_CPP_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_REQUIRED_FLAGS "-lSDL2 -lSDL2_ttf -lSDL2_image -lm")

file(GLOB SOURCES "src/*.cpp")

add_executable(svte ${SOURCES})
target_compile_options(svte PRIVATE -Wall -Wextra -pedantic)

target_link_libraries(svte m)

find_library(SDL2_LIBRARY NAMES SDL2)
find_library(SDL2_LIBRARY_MAIN NAMES SDL2main)
find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf)

find_path(SDL2_INC NAMES SDL2/ SDL.h)
find_path(sndfile_INC NAMES sndfile.h sndfile/)

message("==INC==")
if(NOT SDL2_INC)
  message(FATAL_ERROR "SDL2 includes not found!")
else()
  message(STATUS "SDL2 includes found at ${SDL2_INC}")
  include_directories(${SDL2_INC})
endif()
message("==INC==")

message("===LIB===")
if(NOT SDL2_LIBRARY)
  message(FATAL_ERROR "SDL2 not found, please install it via your package manager!")
else()
  message(STATUS "SDL2 found at ${SDL2_LIBRARY}")
  target_link_libraries(svte ${SDL2_LIBRARY})
endif()

if(NOT SDL2_LIBRARY_MAIN)
  message(WARNING "SDL2main not found!")
else()
  message(STATUS "SDL2main found at ${SDL2_LIBRARY_MAIN}")
endif()

if(NOT SDL2_TTF_LIBRARY)
  message(FATAL_ERROR "SDL2_ttf not found, please install it via your package manager!")
else()
  message(STATUS "SDL2_ttf found at ${SDL2_TTF_LIBRARY}")
  target_link_libraries(svte ${SDL2_TTF_LIBRARY})
endif()
message("===LIB===")




