cmake_minimum_required(VERSION 3.16)

project(EchoProtocol)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_ttf)

set(SRC_FILES
        main.cpp
        src/core/core.cpp
        src/core/logic.cpp
        src/core/render.cpp
        src/scene/scene.cpp
        src/scene/menu_scene.cpp
        src/scene/game_scene.cpp
        src/entities/monster.cpp
        src/entities/bait.cpp
        src/entities/echo.cpp
        src/entities/camera.cpp
        src/entities/lamp.cpp
        src/core/nightConfig.cpp
)

set(APP_ICON_MACOS "${CMAKE_SOURCE_DIR}/resources/iconApp.icns")
set(APP_ICON_WINDOWS "${CMAKE_SOURCE_DIR}/resources/icon.rc")

if(APPLE)
    set_source_files_properties(${APP_ICON_MACOS} PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources"
    )
    add_executable(EchoProtocol MACOSX_BUNDLE
            ${SRC_FILES}
            ${APP_ICON_MACOS}
    )
elseif(WIN32)
    add_executable(EchoProtocol WIN32
            ${SRC_FILES}
            ${APP_ICON_WINDOWS}
    )
else()
    add_executable(EchoProtocol
            ${SRC_FILES}
    )
endif()

target_include_directories(EchoProtocol PRIVATE
        .
        src
        src/headers
)

target_link_libraries(EchoProtocol PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)

target_compile_definitions(EchoProtocol PRIVATE SDL_MAIN_USE_CALLBACKS=1)

# =========================
# macOS bundle settings
# =========================
if(APPLE)
    set_target_properties(EchoProtocol PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_NAME "Echo Protocol"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.fevil.echoprotocol"
            MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
            MACOSX_BUNDLE_ICON_FILE "iconApp.icns"
            BUILD_WITH_INSTALL_RPATH TRUE
            INSTALL_RPATH "@executable_path/../Frameworks"
    )

    add_custom_command(TARGET EchoProtocol POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_BUNDLE_CONTENT_DIR:EchoProtocol>/Resources"
            COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_BUNDLE_CONTENT_DIR:EchoProtocol>/Frameworks"

            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_BUNDLE_CONTENT_DIR:EchoProtocol>/Resources/assets"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_BUNDLE_CONTENT_DIR:EchoProtocol>/Frameworks"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3_image::SDL3_image>"
            "$<TARGET_BUNDLE_CONTENT_DIR:EchoProtocol>/Frameworks"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
            "$<TARGET_BUNDLE_CONTENT_DIR:EchoProtocol>/Frameworks"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/external/SDL_image/libSDL3_image.0.5.0.dylib"
            "$<TARGET_BUNDLE_CONTENT_DIR:EchoProtocol>/Frameworks/libSDL3_image.0.dylib"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/external/SDL_ttf/libSDL3_ttf.0.3.0.dylib"
            "$<TARGET_BUNDLE_CONTENT_DIR:EchoProtocol>/Frameworks/libSDL3_ttf.0.dylib"
    )
else()
    add_custom_command(TARGET EchoProtocol POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "${CMAKE_BINARY_DIR}/assets"
    )
endif()

# =========================
# install rules
# =========================
if(APPLE)
    install(TARGETS EchoProtocol BUNDLE DESTINATION .)

    install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets"
            DESTINATION "EchoProtocol.app/Contents/Resources"
    )

    install(FILES
            "$<TARGET_FILE:SDL3::SDL3>"
            "$<TARGET_FILE:SDL3_image::SDL3_image>"
            "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
            DESTINATION "EchoProtocol.app/Contents/Frameworks"
    )
elseif(WIN32)
    install(TARGETS EchoProtocol RUNTIME DESTINATION .)
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets" DESTINATION .)
else()
    install(TARGETS EchoProtocol RUNTIME DESTINATION .)
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets" DESTINATION .)
endif()