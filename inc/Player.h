#pragma once

#include <wchar.h>

#include "TypeDefines.h"
#include "Vector2.h"
#include "CircleCollider.h"
#include "Shape.h"

typedef enum PlayerGear
{
	GEAR_CANON			= 0b00000001,	// 1
	GEAR_MACHINE_GUN	= 0b00000010,	// 2
	GEAR_SHEILD			= 0b00000100	// 4
} PlayerGear;

typedef struct UnionList List;

typedef struct Player
{
	long long id;
	union {
		Vector2 position;
		CircleCollider collider;
	};
	float speed;
	float fire_rate;
	float canon_fire_rate;
	int hp;
	BOOL is_destroyed;
	int gear_state;
	ShapeType shape_type;
	float machine_gun_timer;
	float canon_timer;
	float flame_timer;
} Player;

Player* CreatePlayer();

void UpdatePlayer(Player* player);

void RenderPlayer(Player* player);

void DeletePlayer(Player** player);

void PlayerTakeDamage(Player* player, int damage);

void DestroyPlayer(Player* player);