#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "TypeDefines.h"

typedef enum EffectName
{
	effect_canon_hit,
	//MACHINEGUN_HIT_EFFECT,
	effect_player_flame,
	effect_canon_flame,
	effect_enemy_destroy,
	effect_boss_destroy,
	EFFECT_MAX
} EffectName;

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
	EffectName name;
} Effect;

void InitializeEffectData();

void CreateEffect(Effect* effect, const Vector2* position, EffectName name);

void UpdateEffect(Effect* effect);

void RenderEffect(Effect* effect);

BOOL IsEffectDestroyed(Effect* effect);

void ReleaseEffectData();