#pragma once

#include <wchar.h>

#include "Types.h"
#include "Vector2.h"
#include "CircleCollider.h"

typedef struct UnionList List;
typedef struct SwordfishII SwordfishII;

typedef struct Enemy
{
	long long id;
	union {
		Vector2 position;
		CircleCollider collider;
	};
	int hp;
	BOOL is_destroyed;
	wchar_t shape[2];
} Enemy;

void CreateEnemy(Enemy* enemy);

void UpdateEnemy(Enemy* enemy);

void RenderEnemy(Enemy* enemy);

void DeleteEnemy(Enemy* enemy);

void SetEnemyBulletList(List* bullet_list);

void SetSwordFish(SwordfishII* swordfish);

void EnemyTakeDamage(Enemy* enemy, int damage);

void DestroyEnemy(Enemy* enemy);

BOOL IsEnemyDestroyed(Enemy* enemy);