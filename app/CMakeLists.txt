cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

project(mesh2audio LANGUAGES C CXX OBJC)

add_subdirectory(lib)

file(
    GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp
    lib/implot/implot.cpp lib/implot/implot_items.cpp lib/implot/implot_demo.cpp
    lib/fmt/src/format.cc
    lib/imgui/misc/cpp/imgui_stdlib.cpp
)

# Recursively copy the res directory to the build directory.
file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(lib/implot lib/glm lib/miniaudio lib/fmt/include lib/faust/architecture lib/imgui/misc/cpp)

find_package(GLEW REQUIRED)
# `dynamiclib` is faust.
target_link_libraries(${PROJECT_NAME} PRIVATE IMGUI GLEW::GLEW nfd dynamiclib)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-elaborated-enum-base -DIMGUI_IMPL_OPENGL_LOADER_GLEW)
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS) # ImVec2 & ImVec4 math operators
