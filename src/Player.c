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
#include "Types.h"
#include "Game.h"

static List* player_bullet_list = NULL;
static float player_fire_timer = 0.0f;

static void Translate(Player* player);
static void Fire(Player* player);
static void ScreenBoundCheck(Player* player);

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

	ScreenBoundCheck(player);
}

void RenderPlayer(Player* player)
{
	ScreenDrawString((int)player->position.x, (int)player->position.y, player->shape, FG_RED);
}

void DeletePlayer(Player** player)
{
	player_bullet_list = NULL;

	if (*player != NULL)
	{
		free(*player);

		*player = NULL;
	}
}

void SetBulletList(List* bullet_list)
{
	player_bullet_list = bullet_list;
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

		Insert(player_bullet_list, &bullet, sizeof(Bullet));

		//DebugLog("fire\n");
	}
}

static void ScreenBoundCheck(Player* player)
{
	if (player->position.x < 0)
	{
		player->position.x = 0;
	}

	if (player->position.y < 0)
	{
		player->position.y = 0;
	}

	if (player->position.x > ScreenWidth() - 1)
	{
		player->position.x = (float)ScreenWidth() - 1;
	}

	if (player->position.y > ScreenHeight() - 1)
	{
		player->position.y = (float)ScreenHeight() - 1;
	}
}