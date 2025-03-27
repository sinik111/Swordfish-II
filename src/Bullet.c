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

static void BulletBackFlame(Bullet* bullet);

void CreateCanonBullet(Bullet* bullet, Player* player)
{
	bullet->id = GenerateID();
	vec2 adjustment = { 3.0f, 0.0f };
	bullet->position = AddVector2(&player->position, &adjustment);
	bullet->direction = RightVector;
	bullet->collider = CreateCircleCollider(&bullet->position, 0.5f);
	bullet->speed = 50.0f;
	bullet->timer = 2.0f;
	bullet->damage = 3;
	bullet->is_destroyed = FALSE;
	bullet->has_flame = TRUE;
	bullet->back_flame_timer = 0.0f;
	bullet->type = PLAYER_CANON;
	wmemcpy_s(bullet->shape, 2, L"⬤", 2);
}

void CreateBullet(Bullet* bullet, Player* player)
{
	bullet->id = GenerateID();
	bullet->position = AddVector2(&player->position, &RightVector);
	bullet->direction = RightVector;
	bullet->collider = CreateCircleCollider(&bullet->position, 0.5f);
	bullet->speed = 100.0f;
	bullet->timer = 2.0f;
	bullet->damage = 1;
	bullet->is_destroyed = FALSE;
	bullet->has_flame = TRUE;
	bullet->back_flame_timer = 0.0f;
	bullet->type = PLAYER_CANON;
	wmemcpy_s(bullet->shape, 2, L"-", 2);
}

void CreateEnemyBullet(Bullet* bullet, const Vector2* position)
{
	bullet->id = GenerateID();
	bullet->position = AddVector2(position, &LeftVector);
	//bullet->direction = LeftVector;
	bullet->collider = CreateCircleCollider(&bullet->position, 0.5f);
	bullet->speed = 20.0f;
	bullet->timer = 10.0f;
	bullet->damage = 1;
	bullet->is_destroyed = FALSE;
	wmemcpy_s(bullet->shape, 2, L"●", 2);
}

void UpdateBullet(Bullet* bullet)
{
	vec2 transition = ScalarMulVector2Each(&bullet->direction, bullet->speed * 2 * DeltaTime(), bullet->speed * DeltaTime());
	bullet->position = AddVector2(&bullet->position, &transition);
	bullet->timer -= DeltaTime(); // 자동 삭제 되도록 타이머 시간 감소.

	switch (bullet->type)
	{
	case PLAYER_CANON:
		BulletBackFlame(bullet);
		break;
	default:
		break;
	}

	if (bullet->timer <= 0.0f)
	{
		DestroyBullet(bullet);
	}
}

void RenderBullet(Bullet* bullet)
{
	if (bullet->type == PLAYER_CANON)
	{
		ScreenDrawString((int)bullet->position.x, (int)bullet->position.y, bullet->shape, FG_BLUE);
	}
	else
	{
		ScreenDrawString((int)bullet->position.x, (int)bullet->position.y, bullet->shape, FG_WHITE);
	}
}

void DestroyBullet(Bullet* bullet)
{
	bullet->is_destroyed = TRUE;
}

BOOL IsBulletDestroyed(Bullet* bullet)
{
	return bullet->is_destroyed;
}

void PlayBulletHitEffect(Bullet* bullet)
{
	Effect effect;

	CreateEffect(&effect, &bullet->position, BULLET_HIT_EFFECT);
	Insert(GetEffectList(), &effect, sizeof(Effect));
}

static void BulletBackFlame(Bullet* bullet)
{
	bullet->back_flame_timer += DeltaTime();
	if (bullet->back_flame_timer > 0.05f)
	{
		Effect effect;

		CreateEffect(&effect, &bullet->position, CANON_FLAME_EFFECT);

		Insert(GetEffectList(), &effect, sizeof(Effect));

		bullet->back_flame_timer -= 0.05f;
	}
}