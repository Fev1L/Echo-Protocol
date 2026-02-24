//
//  CMakeLists.txt
//  Echo Protocol
//
//  Created by Fev1L on 25.02.2026.
//

cmake_minimum_required(VERSION 3.16)
project(Echo Protocol)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(SDL3 REQUIRED)
find_package(SDL3_mixer REQUIRED)

target_link_libraries(your_app
    SDL3::SDL3
    SDL3_mixer::SDL3_mixer
)
