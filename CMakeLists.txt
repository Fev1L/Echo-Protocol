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
        src/core/core.cpp
        src/core/logic.cpp
        src/core/render.cpp
        src/scene/scene.cpp
        src/scene/menu_scene.cpp
        src/scene/game_scene.cpp
        src/entities/entities.h
        src/entities/monster.cpp
        src/entities/bait.cpp
        src/entities/echo.cpp
        src/entities/camera.cpp
        src/entities/lamp.cpp
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

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
)

add_dependencies(EchoProtocol copy_assets)