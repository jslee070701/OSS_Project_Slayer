# 플레이어 이동 테스트 프로그램
add_executable(
    PlayerMovementTest
    tests/player_movement_test.cpp
    src/player/Player.cpp
)

# 테스트 프로그램 헤더 경로
target_include_directories(
    PlayerMovementTest
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# raylib 연결
target_link_libraries(
    PlayerMovementTest
    PRIVATE raylib
)

# 컴파일 옵션
if(MSVC)
    target_compile_options(OSS_Project_Slayer PRIVATE /W4 /utf-8)
    target_compile_options(PlayerMovementTest PRIVATE /W4 /utf-8)
endif()