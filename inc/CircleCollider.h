#pragma once

#include "Vector2.h"
#include "TypeDefines.h"

typedef struct CircleCollider
{
	Vector2 position;
	float radius;
} CircleCollider;

CircleCollider CreateCircleCollider(const Vector2* position, float radius);

BOOL IsCollide(const CircleCollider* a, const CircleCollider* b);