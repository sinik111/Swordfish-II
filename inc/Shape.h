#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "TypeDefines.h"

typedef enum ShapeName
{
	shape_player,
	shape_enemy_1,
	shape_enemy_2,
	shape_enemy_3,
	shape_boss_ship,
	shape_boss_gun,
	shape_item_center,
	shape_item_pink,
	shape_item_yellow,
	shape_item_sky,
	shape_player_shield,
	shape_beam_center,
	shape_beam_side,
	SHAPE_MAX
} ShapeName;

typedef struct UnionList List;

typedef struct ShapeData
{
	long long id;
	List* shape_frame_list;
	int frames;
	WORD attribute;
} ShapeData;

void InitializeShapeData();

void RenderShape(const Vector2* position, ShapeName name, int frame);

void ReleaseShapeData();