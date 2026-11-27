#include "monsters/combat.h"

bool CheckCircleCollision(Vector2 posA, float radiusA, Vector2 posB, float radiusB) {
    if (radiusA < 0.0f || radiusB < 0.0f) return false;
    return CheckCollisionCircles(posA, radiusA, posB, radiusB);
}

float ResolvePlayerMonsterCombat(Monster monsters[], Boss* boss, Vector2 playerPos, float playerCollisionRadius) {
    float totalDamage = 0.0f;

    // 1. 일반 몬스터 충돌 판정
    if (monsters != NULL) {
        for (int i = 0; i < MAX_MONSTERS; i++) {
            if (!monsters[i].active) continue;

            if (CheckCircleCollision(playerPos, playerCollisionRadius, monsters[i].position, monsters[i].collisionRadius)) {
                if (monsters[i].attackCooldown <= 0.0f) {
                    totalDamage += monsters[i].attack;
                    monsters[i].attackCooldown = MONSTER_ATTACK_COOLDOWN;
                }
            }
        }
    }

    // 2. 보스 충돌 판정
    if (boss != NULL && boss->active) {
        if (CheckCircleCollision(playerPos, playerCollisionRadius, boss->position, boss->collisionRadius)) {
            if (boss->attackCooldown <= 0.0f) {
                totalDamage += boss->attack;
                boss->attackCooldown = MONSTER_ATTACK_COOLDOWN;
            }
        }
    }

    return totalDamage;
}

MonsterHitResult ResolveProjectileMonsterCombat(Monster monsters[], Boss* boss, Vector2 projectilePos, float projectileRadius, float damage) {
    // 1. 보스 우선 충돌 체크 (덩치가 크므로 먼저 판정)
    if (boss != NULL && boss->active) {
        if (CheckCircleCollision(projectilePos, projectileRadius, boss->position, boss->collisionRadius)) {
            return DamageBoss(boss, damage);
        }
    }

    // 2. 일반 몬스터 충돌 체크
    if (monsters != NULL) {
        for (int i = 0; i < MAX_MONSTERS; i++) {
            if (!monsters[i].active) continue;

            if (CheckCircleCollision(projectilePos, projectileRadius, monsters[i].position, monsters[i].collisionRadius)) {
                return DamageMonster(&monsters[i], damage);
            }
        }
    }

    MonsterHitResult emptyResult = { false, false, 0, { 0.0f, 0.0f }, false, BOSS_MIDTERM };
    return emptyResult;
}