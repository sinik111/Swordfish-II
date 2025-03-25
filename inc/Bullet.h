#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "CircleCollider.h"
#include "TypeDefines.h"

typedef struct Player Player;
typedef struct Enemy Enemy;
typedef struct UnionList List;

typedef struct Bullet
{
	long long id;
	union {
		Vector2 position;
		CircleCollider collider;
	};
	Vector2 direction;
	float x_speed;
	float y_speed;
	float timer;
	int damage;
	BOOL is_destroyed;
	wchar_t shape[2];
} Bullet;

void CreateBullet(Bullet* bullet, Player* player);

void CreateEnemyBullet(Bullet* bullet, Enemy* enemy);

void UpdateBullet(Bullet* bullet);

void RenderBullet(Bullet* bullet);

void DestroyBullet(Bullet* bullet);

BOOL IsBulletDestroyed(Bullet* bullet);

void DeleteBullet(Bullet* bullet);

void SetEffectBulletHitList(List* list);

void PlayBulletHitEffect(Bullet* bullet);