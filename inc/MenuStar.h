#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "TypeDefines.h"

typedef struct MenuStar
{
	long long id;
	vec2 position;
	vec2 direction;
	float speed;
	float acceleration;
	float indicator;
	wchar_t shape[2];
	BOOL is_destroyed;
} MenuStar;

void CreateMenuStar(MenuStar* star);

void UpdateMenuStar(MenuStar* star);

void RenderMenuStar(MenuStar* star);

BOOL IsStarDestroyed(MenuStar* star);