#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "TypeDefines.h"

typedef enum ShapeType
{
	PLAYER_SHAPE,
	ENEMY_1_SHAPE,
	BOSS_1_SHAPE,
	BOSS_2_SHAPE,
	//BOSS_3_SHAPE,
	//BOSS_4_SHAPE,
	SHAPE_MAX
} ShapeType;

typedef struct UnionList List;

typedef struct ShapeData
{
	long long id;
	List* shape_frame_list;
	int frames;
	WORD attribute;
} ShapeData;

void InitializeShapeData();

void RenderShape(const Vector2* position, ShapeType type, int frame);

void ReleaseShapeData();