#include "Bullet.h"

#include <stdlib.h>
#include <wchar.h>

#include "ConsoleRenderer.h"
#include "Player.h"
#include "Global.h"
#include "MyTime.h"
#include "Vector2.h"
#include "TypeDefines.h"
#include "DebugUtility.h"
#include "Enemy.h"
#include "UnionList.h"
#include "Effect.h"
#include "PlayScene.h"

void CreateBullet(Bullet* bullet, Player* player)
{
	bullet->id = GenerateID();
	bullet->position = AddVector2(&player->position, &RightVector);
	bullet->direction = RightVector;
	bullet->collider = CreateCircleCollider(&bullet->position, 1.0f);
	bullet->x_speed = 100.0f;
	bullet->y_speed = 0.0f;
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
	bullet->x_speed = 20.0f;
	bullet->y_speed = 10.0f;
	bullet->timer = 10.0f;
	bullet->damage = 1;
	bullet->is_destroyed = FALSE;
	wmemcpy_s(bullet->shape, 2, L"●", 2);
}

void UpdateBullet(Bullet* bullet)
{
	vec2 speed_mul = ScalarMulVector2Each(&bullet->direction, bullet->x_speed, bullet->y_speed);
	vec2 transition = ScalarMulVector2(&speed_mul, DeltaTime());
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

void PlayBulletHitEffect(Bullet* bullet)
{
	Effect effect;

	CreateEffect(&effect, &bullet->position, BULLET_HIT_EFFECT);
	Insert(GetEffectList(), &effect, sizeof(Effect));
}