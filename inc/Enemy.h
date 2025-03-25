#pragma once

#include <wchar.h>

#include "TypeDefines.h"
#include "Vector2.h"
#include "CircleCollider.h"
#include "EnemySpawner.h"

typedef struct UnionList List;
typedef struct Player Player;

typedef enum EnemyType
{
	ENEMY_TYPE_0
} EnemyType;

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
	EnemySpawnData spawn_data;
	EnemyType type;
} Enemy;

void CreateEnemy(Enemy* enemy);

void UpdateEnemy(Enemy* enemy);

void RenderEnemy(Enemy* enemy);

void DeleteEnemy(Enemy* enemy);

void EnemyTakeDamage(Enemy* enemy, int damage);

void DestroyEnemy(Enemy* enemy);

BOOL IsEnemyDestroyed(Enemy* enemy);