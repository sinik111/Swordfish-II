#include "CircleCollider.h"

#include "Vector2.h"
#include "Types.h"

CircleCollider CreateCircleCollider(const Vector2* position, float radius)
{
	CircleCollider c = { *position, radius };

	return c;
}

BOOL IsCollide(const CircleCollider* a, const CircleCollider* b)
{
	vec2 sub_vec = SubVector2(&a->position, &b->position);
	float distance = GetVecter2Length(&sub_vec);

	if (a->radius + b->radius >= distance)
	{
		return TRUE;
	}

	return FALSE;
}