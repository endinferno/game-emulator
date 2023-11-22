cmake_minimum_required(VERSION 3.2)

project(game_emulator)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(include)
aux_source_directory(src SRC_LIST)

add_subdirectory(dependencies)

add_executable(${PROJECT_NAME} main.cpp ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
