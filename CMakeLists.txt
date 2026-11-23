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


# 몬스터 모듈 단독 테스트 실행 파일
add_executable(monster_test
    "${CMAKE_CURRENT_SOURCE_DIR}/test/monster_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/monsters/monster.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/monsters/combat.cpp"
)

target_include_directories(monster_test PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(monster_test PRIVATE raylib)

if(MSVC)
    target_compile_options(monster_test PRIVATE /W4 /utf-8)
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


# 패시브 아이템 시각 테스트 프로그램
add_executable(
    PassiveItemsVisualTest
    test/passive_items_visual_test.cpp
    src/items/passive_items.cpp
)

target_include_directories(
    PassiveItemsVisualTest PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(PassiveItemsVisualTest PRIVATE raylib)


# 패시브 아이템 테스트 컴파일 옵션
if(MSVC)
    target_compile_options(PassiveItemsTest PRIVATE /W4 /utf-8)
    target_compile_options(PassiveItemsVisualTest PRIVATE /W4 /utf-8)
endif()


# UI 화면 테스트 프로그램
add_executable(
    UIScreenTest
    test/ui_screen_test.cpp
    src/screens/ui_screen.cpp
)

target_include_directories(
    UIScreenTest
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
    UIScreenTest
    PRIVATE raylib
)


# UI 화면 테스트 컴파일 옵션
if(MSVC)
    target_compile_options(UIScreenTest PRIVATE /W4 /utf-8)
endif()

# App 통합 테스트
add_executable(
    AppTest
    test/app_test.cpp
    src/app/app.cpp
    src/screens/ui_screen.cpp
    src/player/player.cpp
    src/monsters/monster.cpp
    src/monsters/combat.cpp
    src/items/passive_items.cpp
)

target_include_directories(
    AppTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    AppTest PRIVATE
    raylib
)

if(MSVC)
    target_compile_options(AppTest PRIVATE /W4 /utf-8)
endif()