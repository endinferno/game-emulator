cmake_minimum_required(VERSION 3.5)

set(BINARY_NAME GameEmulator)
project(${BINARY_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build Type
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -Werror -DNDEBUG")
endif()

# Dependency
add_subdirectory(dependencies)
find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)

# Include
set(INCLUDE_LIST ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDE_LIST})

# Source
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

# Executable
add_executable(${BINARY_NAME} Main.cpp ${SRC_LIST})
target_link_libraries(${BINARY_NAME} fmt)
target_link_libraries(${BINARY_NAME} sfml-window sfml-graphics sfml-system)
