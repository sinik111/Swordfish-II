#include "Bullet.h"

#include <stdlib.h>
#include <wchar.h>

#include "ConsoleRenderer.h"
#include "SwordfishII.h"
#include "Global.h"
#include "MyTime.h"
#include "Vector2.h"
#include "Types.h"
#include "DebugUtility.h"
#include "Enemy.h"

void CreateBullet(Bullet* bullet, SwordfishII* swordfish)
{
	bullet->id = GenerateID();
	bullet->position = AddVector2(&swordfish->position, &RightVector);
	bullet->direction = RightVector;
	bullet->collider = CreateCircleCollider(&bullet->position, 1.0f);
	bullet->speed = 100.0f;
	bullet->timer = 2.0f;
	bullet->damage = 1;
	bullet->is_destroyed = FALSE;
	wmemcpy_s(bullet->shape, 2, L"-", 2);
}

void CreateEnemyBullet(Bullet* bullet, Enemy* enemy)
{
	bullet->id = GenerateID();
	bullet->position = AddVector2(&enemy->position, &LeftVector);
	bullet->direction = LeftVector;
	bullet->collider = CreateCircleCollider(&bullet->position, 1.0f);
	bullet->speed = 20.0f;
	bullet->timer = 10.0f;
	bullet->damage = 1;
	bullet->is_destroyed = FALSE;
	wmemcpy_s(bullet->shape, 2, L"●", 2);
}

void UpdateBullet(Bullet* bullet)
{
	vec2 transition = ScalarMulVector2Args(&bullet->direction, 2, bullet->speed, DeltaTime());
	bullet->position = AddVector2(&bullet->position, &transition);
	bullet->timer -= DeltaTime(); // 자동 삭제 되도록 타이머 시간 감소.
	if (bullet->timer <= 0.0f)
	{
		DestroyBullet(bullet);
	}
}

void RenderBullet(Bullet* bullet)
{
	ScreenDrawString((int)bullet->position.x, (int)bullet->position.y, bullet->shape, FG_WHITE);
}

void DestroyBullet(Bullet* bullet)
{
	bullet->is_destroyed = TRUE;
}

BOOL IsBulletDestroyed(Bullet* bullet)
{
	return bullet->is_destroyed;
}

void DeleteBullet(Bullet* bullet)
{

}