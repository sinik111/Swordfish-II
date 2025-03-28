#pragma once

#include <wchar.h>

#include "TypeDefines.h"
#include "Vector2.h"
#include "Collision.h"
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
	int shield;
	int shield_max;
	int skill_gauge;
	int skill_gauge_max;
	BOOL is_destroyed;
	int gear_state;
	float machine_gun_timer;
	float canon_timer;
	float flame_timer;
	float charge_timer;
	BOOL is_locked;
	float lock_timer;
	float lock_duration;
	BOOL is_skill_fired;
	float skill_delay_timer;
	float skill_delay_duration;
	int skill_damage;
} Player;

Player* CreatePlayer();

void UpdatePlayer(Player* player);

void RenderPlayer(Player* player);

void DeletePlayer(Player** player);

void PlayerTakeDamage(Player* player, int damage);

void PlayerGetItem(Player* player);

void PlayerRemoveGear(Player* player, PlayerGear gear);

void DestroyPlayer(Player* player);