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
#include "Boss.h"
#include "Beam.h"

static void Translate(Player* player);
static void WeaponUpdate(Player* player);
static void FireCanon(Player* player);
static void FireMachineGun(Player* player);
static void ScreenBoundCheck(Player* player);
static void BackFlame(Player* player);
static void ChargeSkill(Player* player);
static void FireSkill(Player* player);
static void GiveDamageToAll(Player* player);

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
	player->hp = 20;
	player->shield = 0;
	player->shield_max = 10;
	player->fire_rate = 0.1f;
	player->is_destroyed = FALSE;
	player->collider.radius = 1.0f;
	player->canon_fire_rate = 0.5f;
	player->gear_state = GEAR_CANON;
	player->machine_gun_timer = 0.0f;
	player->canon_timer = 0.0f;
	player->flame_timer = 0.0f;
	player->skill_gauge = 0;
	player->skill_gauge_max = 10;
	player->charge_timer = 0.0f;
	player->is_locked = FALSE;
	player->lock_timer = 0.0f;
	player->lock_duration = 1.0f;
	player->is_skill_fired = FALSE;
	player->skill_delay_timer = 0.0f;
	player->skill_delay_duration = 0.6f;
	player->skill_damage = 20;
	
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

	if (!player->is_locked)
	{
		Translate(player);

		WeaponUpdate(player);
	}

	BackFlame(player);

	ScreenBoundCheck(player);

	ChargeSkill(player);

	FireSkill(player);
}

void RenderPlayer(Player* player)
{
	if ((IsKeyDown(VK_UP) || IsKeyDown(VK_DOWN)) && !player->is_locked)
	{
		RenderShape(&player->position, shape_player, 0);
	}
	else
	{
		RenderShape(&player->position, shape_player, 1);
	}

	if (player->gear_state & GEAR_SHEILD)
	{
		vec2 adjusted = AddVector2(&player->position, &LeftVector);
		RenderShape(&adjusted, shape_player_shield, 0);
	}
}

void PlayerTakeDamage(Player* player, int damage)
{
	if (player->gear_state & GEAR_SHEILD)
	{
		player->shield -= damage;
		if (player->shield <= 0)
		{
			player->hp -= player->shield;
			player->shield = 0;

			PlayerRemoveGear(player, GEAR_SHEILD);

			player->collider.radius = 1.0f;
		}
	}
	else
	{
		player->hp -= damage;

		if (player->gear_state & GEAR_MACHINE_GUN)
		{
			PlayerRemoveGear(player, GEAR_MACHINE_GUN);
		}
	}
	
}

void PlayerGetItem(Player* player)
{
	if (!(player->gear_state & GEAR_MACHINE_GUN))
	{
		player->gear_state |= GEAR_MACHINE_GUN;
	}
	else
	{
		player->gear_state |= GEAR_SHEILD;

		player->shield += 10;
		if (player->shield > player->shield_max)
		{
			player->shield = player->shield_max;
		}

		player->collider.radius = 3.0f;
	}
}

void PlayerRemoveGear(Player* player, PlayerGear gear)
{
	player->gear_state ^= gear;
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
	
	if (player->gear_state & GEAR_MACHINE_GUN)
	{
		FireMachineGun(player);
	}
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
	if (IsKeyDown('A'))
	{
		if (player->fire_rate < player->machine_gun_timer)
		{
			player->machine_gun_timer = 0.0f;
			Bullet bullet;

			CreateBulletUpper(&bullet, player);
			Insert(GetPlayerBulletList(), &bullet, sizeof(Bullet));

			CreateBulletLower(&bullet, player);
			Insert(GetPlayerBulletList(), &bullet, sizeof(Bullet));
		}
	}
}

static void FireSkill(Player* player)
{
	if (IsKeyReleased('S'))
	{
		if (player->skill_gauge == player->skill_gauge_max)
		{
			player->charge_timer = 0.0f;
			player->skill_gauge = 0;

			player->is_locked = TRUE;

			player->is_skill_fired = FALSE;

			Effect effect;

			vec2 adjust = { 3.0f, 0.0f };
			vec2 position = AddVector2(&player->position, &adjust);

			CreateEffect(&effect, &position, effect_beam_energy);

			Insert(GetEffectList(), &effect, sizeof(Effect));
		}
	}

	if (player->is_locked)
	{
		player->lock_timer += DeltaTime();
		if (player->lock_timer >= player->lock_duration)
		{
			player->is_locked = FALSE;
			player->lock_timer = 0.0f;
		}

		if (!player->is_skill_fired)
		{
			player->skill_delay_timer += DeltaTime();
			if (player->skill_delay_timer >= player->skill_delay_duration)
			{
				player->is_skill_fired = TRUE;
				player->skill_delay_timer = 0.0f;
				GiveDamageToAll(player);

				Beam* beam = CreateBeam(player);
				SetBeam(beam);
			}
		}
	}
}

static void ScreenBoundCheck(Player* player)
{
	if (player->position.x < 2)
	{
		player->position.x = 2;
	}

	if (player->position.y < 3)
	{
		player->position.y = 3;
	}

	if (player->position.x > ScreenWidth() / 2 + 10)
	{
		player->position.x = (float)(ScreenWidth() / 2 + 10);
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

		CreateEffect(&effect, &position, effect_player_flame);

		Insert(GetEffectList(), &effect, sizeof(Effect));

		player->flame_timer -= 0.05f;
	}
}

static void ChargeSkill(Player* player)
{
	if (player->charge_timer < player->skill_gauge_max)
	{
		player->charge_timer += DeltaTime();
	}
	
	if (player->charge_timer >= player->skill_gauge_max)
	{
		player->charge_timer = (float)player->skill_gauge_max;
	}
	
	player->skill_gauge = (int)player->charge_timer;
}

static void GiveDamageToAll(Player* player)
{
	Boss* boss = GetBoss();
	if (boss != NULL)
	{
		BossTakeDamage(boss, player->skill_damage);
	}

	List* enemy_list = GetEnemyList();
	if (enemy_list != NULL)
	{
		Node* current_node = enemy_list->head;
		while (current_node != NULL)
		{
			EnemyTakeDamage(&current_node->data.enemy, player->skill_damage);

			current_node = current_node->next;
		}
	}

	List* bullet_list = GetEnemyBulletList();
	if (bullet_list != NULL)
	{
		ClearList(bullet_list);
	}
}