#pragma once

#include "TypeDefines.h"
#include "Vector2.h"

typedef struct UnionList List;
typedef struct Enemy Enemy;
typedef struct Player Player;
typedef struct Boss Boss;

typedef struct CircleCollider
{
	Vector2 position;
	float radius;
} CircleCollider;

void CheckBulletsToBossCollision(List* bullet_list, Boss* enemy);

void CheckBulletsToEnemiesCollision(List* bullet_list, List* enemy_list);

void CheckPlayerToEnemyBulletsCollision(Player* player, List* enemy_bullet_list);

void CheckPlayerToItemsCollision(Player* player, List* item_list);