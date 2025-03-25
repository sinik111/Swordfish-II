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

static float player_fire_timer = 0.0f;
static float player_flame_timer = 0.0f;

static void Translate(Player* player);
static void Fire(Player* player);
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
	player->x_speed = 60.0f;
	player->y_speed = 30.0f;
	player->hp = 30;
	player->fire_rate = 0.1f;
	player->is_destroyed = FALSE;
	player->collider.radius = 2.0f;
	wmemcpy_s(player->shape, 2, L"▶", 2);
	
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

	if (IsKeyDown('A'))
	{
		Fire(player);
	}

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

	direction.x *= player->x_speed;
	direction.y *= player->y_speed;

	vec2 transition = ScalarMulVector2(&direction, DeltaTime());

	player->position = AddVector2(&player->position, &transition);


}

static void Fire(Player* player)
{
	player_fire_timer += DeltaTime();
	if (player->fire_rate < player_fire_timer)
	{
		player_fire_timer -= player->fire_rate;
		Bullet bullet;
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
	player_flame_timer += DeltaTime();
	if (player_flame_timer > 0.05f)
	{
		Effect effect;

		vec2 back = { -4.0f, 0.0f };
		vec2 position = AddVector2(&player->position, &back);

		CreateEffect(&effect, &position, PLAYER_FLAME_EFFECT);

		Insert(GetEffectList(), &effect, sizeof(Effect));

		player_flame_timer -= 0.05f;
	}
}