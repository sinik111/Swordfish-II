#pragma once

#include "Vector2.h"
#include "Collision.h"
#include "TypeDefines.h"
#include "Shape.h"

typedef enum BossPatternState
{
	STATE_NONE		= 0b0000,
	STATE_NORMAL	= 0b0001,
	STATE_SPREAD	= 0b0010,
	STATE_RAPID		= 0b0100,
	STATE_VOLLEY	= 0b1000
} BossPatternState;

typedef enum BossPatternTypes
{
	PATTERN_NORMAL,
	PATTERN_SPREAD,
	PATTERN_RAPID,
	PATTERN_VOLLEY
} BossPatternTypes;

typedef struct BossTimeData
{
	float fire_rate;
	float fire_timer;
	float pattern_duration;
} BossTimeData;

typedef struct Boss
{
	long long id;
	union {
		Vector2 position;
		CircleCollider collider;
	};
	Vector2 destination;
	Vector2 normal_fire_position;
	Vector2 spread_fire_position;
	Vector2 rapid_fire_position;
	float spread_y;
	float spread_gap;
	float speed;
	BossTimeData pattern_time_data[4];
	float boss_timer;
	float pattern_timer;
	float mid_pattern_timer;
	int boss_pattern_state;
	int hp;
	int empty_position;
	BOOL is_destroyed;
	BOOL is_started;
	ShapeType shape_type[4];
} Boss;

Boss* CreateBoss();

void UpdateBoss(Boss* boss);

void RenderBoss(Boss* boss);

void BossTakeDamage(Boss* boss, int damage);