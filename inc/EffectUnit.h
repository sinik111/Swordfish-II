#pragma once

#include <wchar.h>

#include <Vector2.h>

typedef struct EffectUnit
{
	long long id;
	Vector2 position;
	wchar_t shape;
} EffectUnit;

void CreateEffectUnit(EffectUnit* effect_unit, const Vector2* position, const wchar_t* shape);