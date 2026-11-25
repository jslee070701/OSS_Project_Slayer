
// 헤더 파일이 여러 번 포함되는 것을 방지
#pragma once

// 전체 스테이지 수
constexpr int TOTAL_STAGES = 12;

// 게임 진행 상태
enum class GameState
{
    PLAYING,    // 게임 진행 중
    GAME_OVER,  // 게임 오버
    GAME_CLEAR  // 게임 클리어
};

// 게임 진행 정보
struct GameProgress
{
    int currentStage = 1;                     // 현재 스테이지 번호
    GameState gameState = GameState::PLAYING; // 현재 게임 상태
};

// 스테이지 번호에 해당하는 이름 반환
const char* GetStageName(int stageNumber);

// 다음 스테이지로 진행
bool AdvanceStage(GameProgress& progress);

// 보스 스테이지 여부 확인
bool IsBossStage(int stageNumber);

// 게임 오버 상태로 변경
void SetGameOver(GameProgress& progress);

// 게임 클리어 상태로 변경
void SetGameClear(GameProgress& progress);

// 게임 종료 여부 확인
bool IsGameFinished(const GameProgress& progress);
