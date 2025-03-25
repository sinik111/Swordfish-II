#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "Types.h"

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
	BOOL is_destroyed;
} EffectBulletHit;

void CreateEffectBulletHitData();

void CreateEffectBulletHit(EffectBulletHit* effect);

void UpdateEffectBulletHit(EffectBulletHit* effect);

void RenderEffectBulletHit(EffectBulletHit* effect);

BOOL IsEffectBulletHitDestroyed(EffectBulletHit* effect);

void DeleteEffectBulletHit();