#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "TypeDefines.h"

typedef struct DrawUnit
{
	long long id;
	Vector2 position;
	wchar_t shape;
	WORD attribute;
} DrawUnit;

void CreateDrawUnit(DrawUnit* draw_unit, const Vector2* position, const wchar_t* shape, WORD attribute);