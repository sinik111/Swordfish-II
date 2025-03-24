#pragma once

#include <wchar.h>

#include "Types.h"
#include "Vector2.h"
#include "CircleCollider.h"

typedef struct UnionList List;

typedef struct SwordfishII
{
	long long id;
	union {
		Vector2 position;
		CircleCollider collider;
	};
	float x_speed;
	float y_speed;
	float fire_rate;
	int hp;
	BOOL is_destroyed;
	wchar_t shape[2];
} SwordfishII;

SwordfishII* CreateSwordfishII();

void UpdateSwordfishII(SwordfishII* swordfish);

void RenderSwordfishII(SwordfishII* swordfish);

void DeleteSwordfishII(SwordfishII** swordfish);

void SetBulletList(List* bullet_list);

void SwordfishTakeDamage(SwordfishII* swordfish, int damage);

void DestroySwordfish(SwordfishII* swordfish);