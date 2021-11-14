cmake_minimum_required(VERSION 3.20)
project(labyrinthe C)

set(CMAKE_C_STANDARD 99)
set(SDL2_INCLUDE_DIR C:/lib/SDL2-2.0.16/include)
set(SDL2_LIB_DIR C:/lib/SDL2-2.0.16/lib/x86)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})


add_executable(labyrinthe main.c labyrinthe.c labyrinthe.h fileCoordinate.c fileCoordinate.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)