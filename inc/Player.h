#pragma once

#include <wchar.h>

#include "Types.h"
#include "Vector2.h"
#include "CircleCollider.h"

typedef struct UnionList List;

typedef struct Player
{
	long long id;
	union {
		Vector2 position;
		CircleCollider collider;
	};
	float x_speed;
	float y_speed;
	float fire_rate;
	int hp;
	BOOL is_destroyed;
	wchar_t shape[2];
} Player;

Player* CreatePlayer();

void UpdatePlayer(Player* player);

void RenderPlayer(Player* player);

void DeletePlayer(Player** player);

void SetBulletList(List* bullet_list);

void PlayerTakeDamage(Player* player, int damage);

void DestroyPlayer(Player* player);