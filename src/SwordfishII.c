#include "SwordfishII.h"

#include <stdlib.h>
#include <wchar.h>

#include "Global.h"
#include "ConsoleRenderer.h"
#include "Input.h"
#include "MyTime.h"
#include "UnionList.h"
#include "Vector2.h"
#include "Bullet.h"
#include "DebugUtility.h"
#include "Types.h"
#include "Game.h"

static List* swordfish_bullet_list = NULL;
static float swordfish_fire_timer = 0.0f;

static void Translate(SwordfishII* swordfish);
static void Fire(SwordfishII* swordfish);
static void ScreenBoundCheck(SwordfishII* swordfish);

SwordfishII* CreateSwordfishII()
{
	SwordfishII* swordfish = (SwordfishII*)malloc(sizeof(SwordfishII));
	if (swordfish == NULL)
	{
		return NULL;
	}

	swordfish->id = GenerateID();
	vec2 pos = { 0.0f, (float)ScreenHeight() / 2 };
	swordfish->position = pos;
	swordfish->x_speed = 60.0f;
	swordfish->y_speed = 30.0f;
	swordfish->hp = 10;
	swordfish->fire_rate = 0.1f;
	swordfish->is_destroyed = FALSE;
	swordfish->collider.radius = 2.0f;
	wmemcpy_s(swordfish->shape, 2, L"▶", 2);
	
	return swordfish;
}

void UpdateSwordfishII(SwordfishII* swordfish)
{
	if (swordfish->hp < 0)
	{
		DestroySwordfish(swordfish);

		ChangeScene(END);

		return;
	}

	Translate(swordfish);

	if (IsKeyDown('A'))
	{
		Fire(swordfish);
	}

	ScreenBoundCheck(swordfish);
}

void RenderSwordfishII(SwordfishII* swordfish)
{
	ScreenDrawString((int)swordfish->position.x, (int)swordfish->position.y, swordfish->shape, FG_RED);
}

void DeleteSwordfishII(SwordfishII** swordfish)
{
	swordfish_bullet_list = NULL;

	if (*swordfish != NULL)
	{
		free(*swordfish);

		*swordfish = NULL;
	}
}

void SetBulletList(List* bullet_list)
{
	swordfish_bullet_list = bullet_list;
}

void SwordfishTakeDamage(SwordfishII* swordfish, int damage)
{
	swordfish->hp -= damage;
}

void DestroySwordfish(SwordfishII* swordfish)
{
	swordfish->is_destroyed = TRUE;
}

// player 내에서만 쓰는 함수들
static void Translate(SwordfishII* swordfish)
{
	vec2 direction = ZeroVector;
	if (IsKeyDown(VK_UP))
	{
		direction = AddVector2(&UpVector, &direction);
	}

	if (IsKeyDown(VK_DOWN))
	{
		direction = AddVector2(&DownVector, &direction);
	}

	if (IsKeyDown(VK_RIGHT))
	{
		direction = AddVector2(&RightVector, &direction);
	}

	if (IsKeyDown(VK_LEFT))
	{
		direction = AddVector2(&LeftVector, &direction);
	}

	NormalizeVector2(&direction);

	direction.x *= swordfish->x_speed;
	direction.y *= swordfish->y_speed;

	vec2 transition = ScalarMulVector2(&direction, DeltaTime());

	swordfish->position = AddVector2(&swordfish->position, &transition);


}

static void Fire(SwordfishII* swordfish)
{
	swordfish_fire_timer += DeltaTime();
	if (swordfish->fire_rate < swordfish_fire_timer)
	{
		swordfish_fire_timer -= swordfish->fire_rate;
		Bullet bullet;
		CreateBullet(&bullet, swordfish);

		Insert(swordfish_bullet_list, &bullet, sizeof(Bullet));

		DebugLog("fire\n");
	}
}

static void ScreenBoundCheck(SwordfishII* swordfish)
{
	if (swordfish->position.x < 0)
	{
		swordfish->position.x = 0;
	}

	if (swordfish->position.y < 0)
	{
		swordfish->position.y = 0;
	}

	if (swordfish->position.x > ScreenWidth() - 1)
	{
		swordfish->position.x = (float)ScreenWidth() - 1;
	}

	if (swordfish->position.y > ScreenHeight() - 1)
	{
		swordfish->position.y = (float)ScreenHeight() - 1;
	}
}