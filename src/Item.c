#include "Item.h"

#include "stdlib.h"

#include "TypeDefines.h"
#include "Shape.h"
#include "Global.h"
#include "MyTime.h"
#include "Vector2.h"
#include "ConsoleRenderer.h"

void CreateItem(Item* item)
{
	item->id = GenerateID();
	item->shape_change_timer = 0.0f;
	item->shape_change_rate = 0.1f;
	item->shape_choose = 0;

	vec2 position = { (float)(ScreenWidth() + 2), (float)(rand() % 26 + 2) };
	vec2 direction = { -0.5f, rand() % 2 == 0 ? -0.5f : 0.5f };

	item->position = position;
	item->direction = direction;
	item->collider.radius = 2.0f;

	item->is_destroyed = FALSE;

	item->speed = 20.0f;
}

void UpdateItem(Item* item)
{
	item->shape_change_timer += DeltaTime();
	if (item->shape_change_timer >= item->shape_change_rate)
	{
		item->shape_choose = item->shape_choose == 2 ? 0 : item->shape_choose + 1;

		item->shape_change_timer -= item->shape_change_rate;
	}

	vec2 transition = ScalarMulVector2(&item->direction, item->speed * DeltaTime());
	item->position = AddVector2(&item->position, &transition);

	if (item->position.x < -1)
	{
		item->is_destroyed = TRUE;
	}

	if (item->position.y < 2)
	{
		item->position.y = 2;
		item->direction.y *= -1;
	}

	if (item->position.y > ScreenHeight() - 3)
	{
		item->position.y = (float)(ScreenHeight() - 3);
		item->direction.y *= -1;
	}
}

void RenderItem(Item* item)
{
	//item
	RenderShape(&item->position, shape_item_center, 0);

	switch (item->shape_choose)
	{
	case 0:
		RenderShape(&item->position, shape_item_pink, 0);
		break;
		
	case 1:
		RenderShape(&item->position, shape_item_yellow, 0);
		break;

	case 2:
		RenderShape(&item->position, shape_item_sky, 0);
		break;
	}
}

void DestroyItem(Item* item)
{
	item->is_destroyed = TRUE;
}

BOOL IsItemDestroyed(Item* item)
{
	return item->is_destroyed;
}