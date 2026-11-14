
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

# 실행 파일 생성
add_executable(OSS_Project_Slayer ${GAME_SOURCES})

# 헤더 파일 경로
target_include_directories(OSS_Project_Slayer PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# raylib 연결
target_link_libraries(OSS_Project_Slayer PRIVATE raylib)

# 컴파일 옵션
if(MSVC)
    target_compile_options(OSS_Project_Slayer PRIVATE /W4 /utf-8)
endif()
