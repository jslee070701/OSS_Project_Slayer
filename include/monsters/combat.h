#ifndef COMBAT_H
#define COMBAT_H

#include "raylib.h"
#include "monsters/monster.h"
#include <stdbool.h>

bool CheckCircleCollision(Vector2 posA, float radiusA, Vector2 posB, float radiusB);

// 플레이어 vs 일반 몬스터 + 보스 충돌 처리 (총 피격 대미지 반환)
float ResolvePlayerMonsterCombat(Monster monsters[], Boss* boss, Vector2 playerPos, float playerCollisionRadius);

// 투사체 vs 일반 몬스터 + 보스 충돌 처리 (명중/처치 영수증 반환)
MonsterHitResult ResolveProjectileMonsterCombat(Monster monsters[], Boss* boss, Vector2 projectilePos, float projectileRadius, float damage);

#endif // COMBAT_H