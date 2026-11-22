#include "monsters/combat.h"

#define MONSTER_ATTACK_COOLDOWN 1.0f

bool CheckCircleCollision(Vector2 posA, float radiusA, Vector2 posB, float radiusB) {
    if (radiusA < 0.0f || radiusB < 0.0f) return false;
    return CheckCollisionCircles(posA, radiusA, posB, radiusB);
}

float ResolvePlayerMonsterCombat(Monster monsters[], Vector2 playerPos, float playerCollisionRadius) {
    float totalDamage = 0.0f;

    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        if (CheckCircleCollision(playerPos, playerCollisionRadius, monsters[i].position, monsters[i].collisionRadius)) {
            if (monsters[i].attackCooldown <= 0.0f) {
                totalDamage += monsters[i].attack;
                monsters[i].attackCooldown = MONSTER_ATTACK_COOLDOWN;
            }
        }
    }

    return totalDamage;
}

MonsterHitResult ResolveProjectileMonsterCombat(Monster monsters[], Vector2 projectilePos, float projectileRadius, float damage) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        if (CheckCircleCollision(projectilePos, projectileRadius, monsters[i].position, monsters[i].collisionRadius)) {
            return DamageMonster(monsters[i], damage);
        }
    }

    MonsterHitResult emptyResult = { false, false, 0, { 0.0f, 0.0f } };
    return emptyResult;
}