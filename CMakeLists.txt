cmake_minimum_required(VERSION 3.16)

project(EchoProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_ttf)

file(GLOB_RECURSE SRC_FILES
        *.cpp
        src/*.cpp
)

add_executable(EchoProtocol
        main.cpp
        src/code/init.cpp
        src/code/logic.cpp
        src/code/render.cpp
        src/code/scene.cpp
        src/code/update.cpp
)

target_include_directories(EchoProtocol PRIVATE
        .
        src
        src/headers
)

target_link_libraries(EchoProtocol
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})