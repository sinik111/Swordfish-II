#pragma once

#include <wchar.h>

#include "Types.h"
#include "Vector2.h"
#include "CircleCollider.h"
#include "Shape.h"

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
	ShapeType shape_type;
} Player;

Player* CreatePlayer();

void UpdatePlayer(Player* player);

void RenderPlayer(Player* player);

void DeletePlayer(Player** player);

void PlayerTakeDamage(Player* player, int damage);

void DestroyPlayer(Player* player);