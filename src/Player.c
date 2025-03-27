#include "Player.h"

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
#include "TypeDefines.h"
#include "Game.h"
#include "PlayScene.h"

static void Translate(Player* player);
static void WeaponUpdate(Player* player);
static void FireCanon(Player* player);
static void FireMachineGun(Player* player);
static void ScreenBoundCheck(Player* player);
static void BackFlame(Player* player);

Player* CreatePlayer()
{
	Player* player = (Player*)malloc(sizeof(Player));
	if (player == NULL)
	{
		return NULL;
	}

	player->id = GenerateID();
	vec2 pos = { 0.0f, (float)ScreenHeight() / 2 };
	player->position = pos;
	player->speed = 30.0f;
	player->hp = 30;
	player->fire_rate = 0.2f;
	player->is_destroyed = FALSE;
	player->collider.radius = 0.5f;
	player->canon_fire_rate = 0.5f;
	player->gear_state = GEAR_CANON;
	player->machine_gun_timer = 0.0f;
	player->canon_timer = 0.0f;
	player->flame_timer = 0.0f;
	
	return player;
}

void UpdatePlayer(Player* player)
{
	if (player->hp == 0)
	{
		DestroyPlayer(player);

		ChangeScene(END);

		return;
	}

	Translate(player);

	WeaponUpdate(player);

	BackFlame(player);

	ScreenBoundCheck(player);
}

void RenderPlayer(Player* player)
{
	if (IsKeyDown(VK_UP))
	{
		RenderShape(&player->position, PLAYER_SHAPE, 0);
	}
	else if (IsKeyDown(VK_DOWN))
	{
		RenderShape(&player->position, PLAYER_SHAPE, 0);
	}
	else
	{
		RenderShape(&player->position, PLAYER_SHAPE, 1);
	}
	
}

void DeletePlayer(Player** player)
{
	if (*player != NULL)
	{
		free(*player);

		*player = NULL;
	}
}

void PlayerTakeDamage(Player* player, int damage)
{
	player->hp -= damage;

	//DebugLog("Player HP: %d\n", player->hp);
}

void DestroyPlayer(Player* player)
{
	player->is_destroyed = TRUE;
}

// player 내에서만 쓰는 함수들
static void Translate(Player* player)
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

	direction.x *= (player->speed * 2);
	direction.y *= player->speed;

	vec2 transition = ScalarMulVector2(&direction, DeltaTime());

	player->position = AddVector2(&player->position, &transition);


}

static void WeaponUpdate(Player* player)
{
	if (player->gear_state & GEAR_CANON)
	{
		FireCanon(player);
	}
	
	//if (player->gear_state & GEAR_MACHINE_GUN)
	//{
	//	FireMachineGun(player);
	//}
	//
	//if (player->gear_state & GEAR_MACHINE_GUN)
	//{
	//	FireMachineGun(player);
	//}
}

static void FireCanon(Player* player)
{
	player->canon_timer += DeltaTime();
	if (IsKeyDown('A'))
	{
		if (player->canon_fire_rate < player->canon_timer)
		{
			player->canon_timer = 0;
			Bullet bullet;
			CreateCanonBullet(&bullet, player);
			//CreateBullet(&bullet, player);
		
			Insert(GetPlayerBulletList(), &bullet, sizeof(Bullet));

			//DebugLog("fire\n");
		}
	}
}

static void FireMachineGun(Player* player)
{
	player->machine_gun_timer += DeltaTime();
	if (player->fire_rate < player->machine_gun_timer)
	{
		player->machine_gun_timer -= player->fire_rate;
		Bullet bullet;
		//CreateCanonBullet(&bullet, player);
		CreateBullet(&bullet, player);

		Insert(GetPlayerBulletList(), &bullet, sizeof(Bullet));

		//DebugLog("fire\n");
	}
}

static void ScreenBoundCheck(Player* player)
{
	if (player->position.x < 2)
	{
		player->position.x = 2;
	}

	if (player->position.y < 2)
	{
		player->position.y = 2;
	}

	if (player->position.x > ScreenWidth() - 3)
	{
		player->position.x = (float)(ScreenWidth() - 3);
	}

	if (player->position.y > ScreenHeight() - 3)
	{
		player->position.y = (float)(ScreenHeight() - 3);
	}
}

static void BackFlame(Player* player)
{
	player->flame_timer += DeltaTime();
	if (player->flame_timer > 0.05f)
	{
		Effect effect;

		vec2 back = { -4.0f, 0.0f };
		vec2 position = AddVector2(&player->position, &back);

		CreateEffect(&effect, &position, PLAYER_FLAME_EFFECT);

		Insert(GetEffectList(), &effect, sizeof(Effect));

		player->flame_timer -= 0.05f;
	}
}