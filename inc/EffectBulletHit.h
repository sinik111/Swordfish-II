#pragma once

#include <wchar.h>

#include "Vector2.h"

typedef struct UnionList List;

typedef struct EffectBulletHitData
{
	long long id;
	List* effect_frame_list;
	int frames;
	float duration;
} EffectBulletHitData;

typedef struct EffectBulletHit
{
	long long id;
	Vector2 position;
	float timer;
} EffectBulletHit;

EffectBulletHitData* CreateEffectBulletHitData();