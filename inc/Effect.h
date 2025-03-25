#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "Types.h"

typedef enum EffectType
{
	BULLET_HIT,
} EffectType;

typedef struct UnionList List;

typedef struct EffectData
{
	long long id;
	List* effect_frame_list;
	int frames;
	float duration;
} EffectData;

typedef struct Effect
{
	long long id;
	Vector2 position;
	float timer;
	BOOL is_destroyed;
	EffectType type;
} Effect;

void InitializeEffectData();

void CreateEffect(Effect* effect, const Vector2* position, EffectType type);

void UpdateEffect(Effect* effect);

void RenderEffect(Effect* effect);

BOOL IsEffectDestroyed(Effect* effect);

void ReleaseEffectData();