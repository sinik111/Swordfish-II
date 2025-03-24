#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "CircleCollider.h"
#include "Types.h"

typedef struct SwordfishII SwordfishII;
typedef struct Enemy Enemy;

typedef struct Bullet
{
	long long id;
	union {
		Vector2 position;
		CircleCollider collider;
	};
	Vector2 direction;
	float speed;
	float timer;
	int damage;
	BOOL is_destroyed;
	wchar_t shape[2];
} Bullet;

void CreateBullet(Bullet* bullet, SwordfishII* swordfish);

void CreateEnemyBullet(Bullet* bullet, Enemy* enemy);

void UpdateBullet(Bullet* bullet);

void RenderBullet(Bullet* bullet);

void DestroyBullet(Bullet* bullet);

BOOL IsBulletDestroyed(Bullet* bullet);

void DeleteBullet(Bullet* bullet);