// Core 모듈 헤더 포함
#include "core/core.h"

// 스테이지 번호에 해당하는 이름 반환
const char* GetStageName(int stageNumber)
{
    // 스테이지 번호에 따라 이름 반환
    switch (stageNumber)
    {
    case 1:  return "중간고사 5주 전";
    case 2:  return "중간고사 4주 전";
    case 3:  return "중간고사 3주 전";
    case 4:  return "중간고사 2주 전";
    case 5:  return "중간고사 1주 전";
    case 6:  return "중간고사 D-day";

    case 7:  return "기말고사 5주 전";
    case 8:  return "기말고사 4주 전";
    case 9:  return "기말고사 3주 전";
    case 10: return "기말고사 2주 전";
    case 11: return "기말고사 1주 전";
    case 12: return "기말고사 D-day";

    // 유효하지 않은 스테이지 번호 처리
    default:
        return "잘못된 스테이지";
    }
}

// 다음 스테이지로 진행
bool AdvanceStage(GameProgress& progress)
{
    // 마지막 스테이지에 도달한 경우 진행 중단
    if (progress.currentStage >= TOTAL_STAGES)
        return false;

    // 현재 스테이지 증가
    progress.currentStage++;

    return true;
}

// 보스 스테이지 여부 확인
bool IsBossStage(int stageNumber)
{
    // 중간고사 및 기말고사 D-day 확인
    return stageNumber == 6 || stageNumber == 12;
}