#ifndef COMBAT_H
#define COMBAT_H

#include "raylib.h"
#include "monsters/monster.h"

// 공통 원형 충돌 판정 함수
bool CheckCircleCollision(Vector2 posA, float radiusA, Vector2 posB, float radiusB);

// 플레이어-몬스터 접촉 충돌 판정 (플레이어가 입을 총 피해량 반환, 실제 HP 차감은 Player 모듈에서 수행)
float ResolvePlayerMonsterCombat(Monster monsters[], Vector2 playerPos, float playerCollisionRadius);

// 투사체-몬스터 충돌 판정 및 피해 처리 연결 (DamageMonster 호출 후 결과 반환)
MonsterHitResult ResolveProjectileMonsterCombat(Monster monsters[], Vector2 projectilePos, float projectileRadius, float damage);

#endif