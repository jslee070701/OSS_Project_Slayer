cmake_minimum_required(VERSION 3.22)

project(OSS_Project_Slayer LANGUAGES C CXX)

# C++17 사용
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# raylib 6.2 다운로드 및 연결
include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.2
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

# src 내부의 C++ 파일 자동 등록
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# 기존 게임 실행 파일 생성
add_executable(OSS_Project_Slayer ${GAME_SOURCES})

# 기존 게임 헤더 파일 경로
target_include_directories(OSS_Project_Slayer PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# 기존 게임에 raylib 연결
target_link_libraries(OSS_Project_Slayer PRIVATE raylib)

# 플레이어 이동 테스트 프로그램
add_executable(
    PlayerMovementTest
    test/player_movement_test.cpp
    src/player/player.cpp
)

# 테스트 프로그램 헤더 파일 경로
target_include_directories(
    PlayerMovementTest
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# 테스트 프로그램에 raylib 연결
target_link_libraries(
    PlayerMovementTest
    PRIVATE raylib
)

# 컴파일 옵션
if(MSVC)
    target_compile_options(OSS_Project_Slayer PRIVATE /W4 /utf-8)
    target_compile_options(PlayerMovementTest PRIVATE /W4 /utf-8)
endif()

# 패시브 아이템 테스트
add_executable(
    PassiveItemsTest
    test/passive_items_test.cpp
    src/items/passive_items.cpp
)

target_include_directories(
    PassiveItemsTest PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(PassiveItemsTest PRIVATE raylib)

if(MSVC)
    target_compile_options(PassiveItemsTest PRIVATE /W4 /utf-8)
endif()