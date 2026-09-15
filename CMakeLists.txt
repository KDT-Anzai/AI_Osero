cmake_minimum_required(VERSION 3.20)
project(NeonOsero VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OpenGL/GLFW 関連の設定
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# ネオン・オセロのメインライブラリ
add_library(neon_osero STATIC
    src/Board.cpp
    src/Piece.cpp
    src/GameEngine.cpp
    src/Renderer.cpp
    src/InputHandler.cpp
)

target_include_directories(neon_osero PUBLIC include)
target_link_libraries(neon_osero PRIVATE OpenGL::GL glfw)

# メインエグゼキュートファイル
add_executable(NeonOsero
    main.cpp
    $<TARGET_OBJECTS:neon_osero>
)

target_link_libraries(NeonOsero PRIVATE neon_oserо glfw)

# テスト
enable_testing()
include(CTest)

add_subdirectory(tests)
