#pragma once

#include <wchar.h>

#include "TypeDefines.h"
#include "Vector2.h"
#include "Collision.h"
#include "EnemySpawner.h"

typedef struct UnionList List;
typedef struct Player Player;

typedef enum EnemyType
{
	ENEMY_TYPE_0 = 0,
} EnemyType;

typedef enum CurrentPoint
{
	END_POINT,
	WAY2_POINT,
	WAY1_POINT
} CurrentPoint;

typedef struct Enemy
{
	long long id;
	union {
		Vector2 position;
		CircleCollider collider;
	};
	int hp;
	BOOL is_destroyed;
	EnemySpawnData spawn_data;
	Vector2 previous_position;
	EnemyType type;
	float way_point_timer;
	BOOL on_waiting;
	float fire_timer;
	short current_point;
} Enemy;

void CreateEnemy(Enemy* enemy);

void UpdateEnemy(Enemy* enemy);

void RenderEnemy(Enemy* enemy);

void DeleteEnemy(Enemy* enemy);

void EnemyTakeDamage(Enemy* enemy, int damage);

void DestroyEnemy(Enemy* enemy);

BOOL IsEnemyDestroyed(Enemy* enemy);