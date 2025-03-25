#pragma once

#include "Vector2.h"

typedef enum EnemyMovementType
{
	STRAIGHT,
	WAYPOINT,
	WAVE
} EnemyMovementType;

typedef enum EnemyType EnemyType;

typedef enum EnemySpeedType
{
	LINEAR,
	EASE_IN,
	EASE_OUT,
	SIN
} EnemySpeedType;

typedef struct EnemySpawnData
{
	long long id;
	EnemyType enemy_type;
	EnemyMovementType enemy_movement_type;
	Vector2 spawn_position;
	Vector2 way1_position;
	Vector2 way2_position;
	Vector2 end_position;
	EnemySpeedType way1_speed_type;
	EnemySpeedType way2_speed_type;
	EnemySpeedType end_speed_type;
	short spawn_time;
	short way_count;
	short way1_speed;
	short way2_speed;
	short end_speed;
	short way1_time;
	short way2_time;
} EnemySpawnData;

void InitializeEnemySpawnData();

void UpdateEnemySpawner();

void ReleaseEnemySpawnData();