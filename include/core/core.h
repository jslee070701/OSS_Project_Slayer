// 헤더 파일이 여러 번 포함되는 것을 방지
#pragma once

// 전체 스테이지 수
constexpr int TOTAL_STAGES = 12;

// 게임 진행 정보
struct GameProgress
{
    int currentStage = 1;  // 현재 스테이지 번호
};

// 스테이지 번호에 해당하는 이름 반환
const char* GetStageName(int stageNumber);