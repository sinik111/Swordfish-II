#include "MenuStar.h"

#include <stdlib.h>

#include "TypeDefines.h"
#include "ConsoleRenderer.h"
#include "Vector2.h"
#include "MyTime.h"
#include "Global.h"

void CreateMenuStar(MenuStar* star)
{
	star->id = GenerateID();
	star->position.x = (float)(rand() % (ScreenWidth() + 60));
	star->position.y = -1.0f;
	vec2 direction = { -3.0f, 1.0f };
	NormalizeVector2(&direction);
	star->direction = direction;
	star->speed = 50.0f;
	star->acceleration = 0.0f;
	star->indicator = 0.0f;
	star->is_destroyed = FALSE;
	wmemcpy_s(star->shape, 2, L"¡Ú", 2);
}

void UpdateMenuStar(MenuStar* star)
{
	star->acceleration = 5 * (star->indicator) * (star->indicator) + 1;
	Vector2 r = ScalarMulVector2(&star->direction, star->speed * star->acceleration * DeltaTime());
	star->position = AddVector2(&star->position, &r);
	if (star->indicator < 1)
	{
		star->indicator += DeltaTime();
	}

	if (star->position.x < 0 || star->position.y > ScreenHeight())
	{
		star->is_destroyed = TRUE;
	}
}

void RenderMenuStar(MenuStar* star)
{
	ScreenDrawString((int)star->position.x, (int)star->position.y, star->shape, FG_WHITE);
}

BOOL IsStarDestroyed(MenuStar* star)
{
	return star->is_destroyed;
}