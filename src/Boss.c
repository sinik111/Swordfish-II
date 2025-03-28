#include "Boss.h"

#include <stdlib.h>

#include "TypeDefines.h"
#include "Global.h"
#include "Vector2.h"
#include "ConsoleRenderer.h"
#include "MyTime.h"
#include "Shape.h"
#include "Bullet.h"
#include "PlayScene.h"
#include "UnionList.h"
#include "Player.h"

static void BossEnter(Boss* boss);
static void BossPattern(Boss* boss);

Boss* CreateBoss()
{
	Boss* boss = (Boss*)malloc(sizeof(Boss));
	if (boss == NULL)
	{
		return NULL;
	}

	boss->id = GenerateID();
	vec2 position = { (float)ScreenWidth() + 10, (float)ScreenHeight() / 2 };
	boss->position = position;
	boss->collider.radius = 15.0f;
	boss->speed = 10.0f;
	boss->is_destroyed = FALSE;
	boss->boss_pattern_state = STATE_NONE;
	boss->hp = 20;
	boss->boss_timer = 0.0f;
	boss->is_started = FALSE;
	boss->mid_pattern_timer = 2.0f;
	vec2 destination = { (float)ScreenWidth() - 10, (float)ScreenHeight() / 2 };
	boss->destination = destination;
	vec2 fire_position = { -16.0f, -11.0f };
	boss->normal_fire_position = fire_position;
	fire_position.x = -15.0f;
	fire_position.y = 0.0f;
	boss->spread_fire_position = fire_position;
	fire_position.x = -16.0f;
	fire_position.y = 10.0f;
	boss->rapid_fire_position = fire_position;
	boss->spread_y = 0.0f;
	boss->spread_gap = 13.0f;
	boss->empty_position = -1;
	boss->pattern_timer = 0.0f;

	BossTimeData normal; // 플레이어 방향 발사
	normal.fire_rate = 0.5f;
	normal.fire_timer = 0.0f;
	normal.pattern_duration = 9999.0f; // 항상 발사

	BossTimeData spread; // 랜덤 방향 부채꼴로 발사
	spread.fire_rate = 0.05f;
	spread.fire_timer = 0.0f;
	spread.pattern_duration = 3.0f;

	BossTimeData rapid; // 플레이어 방향 발사
	rapid.fire_rate = 0.1f;
	rapid.fire_timer = 0.0f;
	rapid.pattern_duration = 2.0f;

	BossTimeData volley; // 구멍 남겨두고 직선 발사
	volley.fire_rate = 0.2f;
	volley.fire_timer = 0.0f;
	volley.pattern_duration = 2.0f;

	boss->pattern_time_data[PATTERN_NORMAL] = normal;
	boss->pattern_time_data[PATTERN_SPREAD] = spread;
	boss->pattern_time_data[PATTERN_RAPID] = rapid;
	boss->pattern_time_data[PATTERN_VOLLEY] = volley;

	return boss;
}

void UpdateBoss(Boss* boss)
{
	if (boss->hp <= 0)
	{
		boss->is_destroyed = TRUE;

		boss->boss_timer += DeltaTime();
		if (boss->boss_timer > 0.5f)
		{
			Effect effect;

			// 104 ~ 119 , 10 ~ 20
			Vector2 effect_position = { (float)(rand() % 30 + 90), (float)(rand() % 30) };

			CreateEffect(&effect, &effect_position, effect_boss_destroy);
			Insert(GetEffectList(), &effect, sizeof(Effect));

			boss->boss_timer = 0.0f;
		}
	}

	if (!boss->is_started)
	{
		BossEnter(boss);
	}
	else if (!boss->is_destroyed)
	{
		BossPattern(boss);

		if (boss->boss_pattern_state & 0b1110)
		{
			boss->pattern_timer += DeltaTime();
			if (boss->boss_pattern_state & STATE_SPREAD)
			{
				if (boss->pattern_timer >= boss->pattern_time_data[PATTERN_SPREAD].pattern_duration)
				{
					boss->pattern_timer = 0.0f;
					boss->boss_pattern_state ^= STATE_SPREAD;
				}
			}

			if (boss->boss_pattern_state & STATE_RAPID)
			{
				if (boss->pattern_timer >= boss->pattern_time_data[PATTERN_RAPID].pattern_duration)
				{
					boss->pattern_timer = 0.0f;
					boss->boss_pattern_state ^= STATE_RAPID;
				}
			}

			if (boss->boss_pattern_state & STATE_VOLLEY)
			{
				if (boss->pattern_timer >= boss->pattern_time_data[PATTERN_VOLLEY].pattern_duration)
				{
					boss->pattern_timer = 0.0f;
					boss->boss_pattern_state ^= STATE_VOLLEY;
					boss->empty_position = -1;
				}
			}
		}
		else
		{
			boss->mid_pattern_timer -= DeltaTime();
			if (boss->mid_pattern_timer <= 0.0f)
			{
				int random_pick = rand() % 3;

				switch (random_pick)
				{
				case 0:
					boss->boss_pattern_state |= STATE_SPREAD;
					break;

				case 1:
					boss->boss_pattern_state |= STATE_RAPID;
					break;

				case 2:
					boss->boss_pattern_state |= STATE_VOLLEY;
					break;
				}

				boss->mid_pattern_timer = 3.0f;
			}
		}
	}
}

void RenderBoss(Boss* boss)
{
	RenderShape(&boss->position, shape_boss_ship, 0);
	RenderShape(&boss->position, shape_boss_gun, 0);
}

void BossTakeDamage(Boss* boss, int damage)
{
	boss->hp -= damage;
}

static void BossEnter(Boss* boss)
{
	float speed = boss->speed;
	vec2 difference = SubVector2(&boss->destination, &boss->position);
	vec2 direction = GetNormalizedVector2(&difference);
	vec2 transition = ScalarMulVector2(&direction, speed * DeltaTime());

	float remain_distance = GetVecter2Length(&difference);

	if (remain_distance < 0.1f)
	{
		boss->position = boss->destination;
		boss->is_started = TRUE;
		boss->boss_pattern_state |= STATE_NORMAL;
	}
	else
	{
		boss->position = AddVector2(&boss->position, &transition);
	}
}

static void BossPattern(Boss* boss)
{
	if (boss->boss_pattern_state & STATE_NORMAL)
	{
		boss->pattern_time_data[PATTERN_NORMAL].fire_timer += DeltaTime();
		if (boss->pattern_time_data[PATTERN_NORMAL].fire_timer >=
			boss->pattern_time_data[PATTERN_NORMAL].fire_rate)
		{
			boss->pattern_time_data[PATTERN_NORMAL].fire_timer -=
				boss->pattern_time_data[PATTERN_NORMAL].fire_rate;

			Bullet bullet;

			Vector2 position = AddVector2(&boss->position, &boss->normal_fire_position);
			CreateEnemyBullet(&bullet, &position);

			vec2 player_position = GetPlayer()->position;
			bullet.direction = MakeDirectionVector2(&player_position, &bullet.position);

			Insert(GetEnemyBulletList(), &bullet, sizeof(Bullet));
		}
	}

	if (boss->boss_pattern_state & STATE_SPREAD)
	{
		boss->pattern_time_data[PATTERN_SPREAD].fire_timer += DeltaTime();
		if (boss->pattern_time_data[PATTERN_SPREAD].fire_timer >=
			boss->pattern_time_data[PATTERN_SPREAD].fire_rate)
		{
			boss->pattern_time_data[PATTERN_SPREAD].fire_timer -=
				boss->pattern_time_data[PATTERN_SPREAD].fire_rate;

			Bullet bullet;

			Vector2 position = AddVector2(&boss->position, &boss->spread_fire_position);
			CreateEnemyBullet(&bullet, &position);

			vec2 destination = { -30.0f, boss->spread_y };
			bullet.direction = MakeDirectionVector2(&destination, &bullet.position);

			Insert(GetEnemyBulletList(), &bullet, sizeof(Bullet));

			if (boss->spread_y <= -50.0f || boss->spread_y >= 80.0f)
			{
				boss->spread_gap *= -1;
			}

			boss->spread_y += boss->spread_gap;
		}
	}

	if (boss->boss_pattern_state & STATE_RAPID)
	{
		boss->pattern_time_data[PATTERN_RAPID].fire_timer += DeltaTime();
		if (boss->pattern_time_data[PATTERN_RAPID].fire_timer >=
			boss->pattern_time_data[PATTERN_RAPID].fire_rate)
		{
			boss->pattern_time_data[PATTERN_RAPID].fire_timer -=
				boss->pattern_time_data[PATTERN_RAPID].fire_rate;

			Bullet bullet;

			Vector2 position = AddVector2(&boss->position, &boss->rapid_fire_position);
			CreateEnemyBullet(&bullet, &position);

			vec2 player_position = GetPlayer()->position;
			bullet.direction = MakeDirectionVector2(&player_position, &bullet.position);

			Insert(GetEnemyBulletList(), &bullet, sizeof(Bullet));
		}
	}

	if (boss->boss_pattern_state & STATE_VOLLEY)
	{
		if (boss->empty_position == -1)
		{
			boss->empty_position = rand() % 23 + 3;
		}

		boss->pattern_time_data[PATTERN_VOLLEY].fire_timer += DeltaTime();
		if (boss->pattern_time_data[PATTERN_VOLLEY].fire_timer >=
			boss->pattern_time_data[PATTERN_VOLLEY].fire_rate)
		{
			boss->pattern_time_data[PATTERN_VOLLEY].fire_timer -=
				boss->pattern_time_data[PATTERN_VOLLEY].fire_rate;

			for (int i = 2; i <= 27; ++i)
			{
				if (i > boss->empty_position - 3 && i < boss->empty_position + 3)
				{
					continue;
				}

				Bullet bullet;

				Vector2 position = { 110.0f, (float)i };
				CreateEnemyBullet(&bullet, &position);

				bullet.direction = LeftVector;

				Insert(GetEnemyBulletList(), &bullet, sizeof(Bullet));
			}
		}
	}
}