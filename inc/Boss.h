#pragma once

#include "Vector2.h"
#include "CircleCollider.h"
#include "TypeDefines.h"
#include "Shape.h"

typedef enum BossPattern
{
	PATTERN_NORMAL_FIRE,
	PATTERN_SPREAD_FIRE,
	PATTERN_RAPID_FIRE,
	PATTERN_VOLLEY_FIRE
} BossPatern;

typedef struct Boss
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
} Boss;