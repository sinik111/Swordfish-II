#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "TypeDefines.h"
#include "Collision.h"

typedef struct Player Player;
typedef struct Enemy Enemy;
typedef struct UnionList List;

typedef enum BulletType
{
	PLAYER_CANON
} BulletType;

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
	BOOL has_flame;
	float back_flame_timer;
	wchar_t shape[2];
	BulletType type;
} Bullet;

void CreateCanonBullet(Bullet* bullet, Player* player);

void CreateBullet(Bullet* bullet, Player* player);

void CreateEnemyBullet(Bullet* bullet, const Vector2* position);

void UpdateBullet(Bullet* bullet);

void RenderBullet(Bullet* bullet);

void DestroyBullet(Bullet* bullet);

BOOL IsBulletDestroyed(Bullet* bullet);

void PlayBulletHitEffect(Bullet* bullet);