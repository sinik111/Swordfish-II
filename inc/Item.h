#pragma once

#include "Vector2.h"
#include "TypeDefines.h"
#include "Shape.h"
#include "Collision.h"

typedef struct Item
{
	long long id;
	union {
		Vector2 position;
		CircleCollider collider;
	};
	BOOL is_destroyed;
	float shape_change_timer;
	float shape_change_rate;
	int shape_choose;
	float speed;
	Vector2 direction;
} Item;

void CreateItem(Item* item);

void UpdateItem(Item* item);

void RenderItem(Item* item);

void DestroyItem(Item* item);

BOOL IsItemDestroyed(Item* item);