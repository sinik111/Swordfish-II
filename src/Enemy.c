#include "Enemy.h"

#include <stdlib.h>

#include "ConsoleRenderer.h"
#include "Global.h"
#include "TypeDefines.h"
#include "Bullet.h"
#include "Vector2.h"
#include "UnionList.h"
#include "MyTime.h"
#include "Player.h"
#include "DebugUtility.h"
#include "Shape.h"
#include "PlayScene.h"
#include "Effect.h"

static float fire_timer = 0.0f;

static void EnemyMovement(Enemy* enemy);

void CreateEnemy(Enemy* enemy)
{
	enemy->id = GenerateID();
	//vec2 pos = { (float)(rand() % ScreenWidth() / 2 + 60), (float)(rand() % ScreenHeight())};
	//enemy->position = pos;
	enemy->collider.radius = 2.0f;
	enemy->hp = 5;
	enemy->is_destroyed = FALSE;
	enemy->position = enemy->spawn_data.spawn_position;
	enemy->type = enemy->spawn_data.enemy_type;
	enemy->way_point_timer = 0.0f;
	enemy->current_point = enemy->spawn_data.way_count;
	enemy->on_waiting = FALSE;
	enemy->fire_timer = 0.0f;
	enemy->previous_position = enemy->position;
	//wmemcpy_s(enemy->shape, 2, L"бс", 2);
}

void UpdateEnemy(Enemy* enemy)
{
	if (!enemy->on_waiting)
	{
		EnemyMovement(enemy);
	}
	else
	{
		enemy->way_point_timer += DeltaTime();

		float wait_time = 0.0f;
		if (enemy->current_point == WAY1_POINT)
		{
			wait_time = enemy->spawn_data.way1_time;
		}
		else if (enemy->current_point == WAY2_POINT)
		{
			wait_time = enemy->spawn_data.way2_time;
		}

		if (enemy->way_point_timer > wait_time)
		{
			--enemy->current_point;
			enemy->on_waiting = FALSE;
		}
	}
	
	if (enemy->hp < 0)
	{
		DestroyEnemy(enemy);
	}

	enemy->fire_timer += DeltaTime();
	if (enemy->fire_timer >= 1.0f)
	{
		Bullet bullet;
		CreateEnemyBullet(&bullet, &enemy->position);
		//bullet.direction = LeftVector;
		vec2 player_position = GetPlayer()->position;
		bullet.direction = MakeDirectionVector2(&player_position, &bullet.position);

		Insert(GetEnemyBulletList(), &bullet, sizeof(Bullet));

		enemy->fire_timer -= 1.0f;
	}
}

void RenderEnemy(Enemy* enemy)
{
	RenderShape(&enemy->position, ENEMY_1_SHAPE, 0);
	//ScreenDrawString((int)enemy->position.x, (int)enemy->position.y, enemy->shape, FG_YELLOW);
}

void DeleteEnemy(Enemy* enemy)
{

}

void EnemyTakeDamage(Enemy* enemy, int damage)
{
	//DebugLog("Enemy %lld HP: %d\n", enemy->id, enemy->hp);
	enemy->hp -= damage;
}

void DestroyEnemy(Enemy* enemy)
{
	enemy->is_destroyed = TRUE;

	Effect effect;
	CreateEffect(&effect, &enemy->position, ENEMY_DESTROY_EFFECT);

	Insert(GetEffectList(), &effect, sizeof(Effect));
}

BOOL IsEnemyDestroyed(Enemy* enemy)
{
	return enemy->is_destroyed;
}

static void EnemyMovement(Enemy* enemy)
{
	switch (enemy->spawn_data.enemy_movement_type)
	{
	case STRAIGHT:
	{
		switch (enemy->spawn_data.end_speed_type)
		{
		case LINEAR:
		{
			float speed = enemy->spawn_data.end_speed;
			vec2 difference = SubVector2(&enemy->spawn_data.end_position, &enemy->position);
			vec2 direction = GetNormalizedVector2(&difference);
			vec2 transition = ScalarMulVector2Each(&direction, speed * 2 * DeltaTime(), speed * DeltaTime());

			float remain_distance = GetVecter2Length(&difference);

			if (remain_distance < 0.1f)
			{
				enemy->position = enemy->spawn_data.end_position;
				DestroyEnemy(enemy);
			}
			else
			{
				enemy->position = AddVector2(&enemy->position, &transition);
			}
		}
		break;
		}
	}
	break;

	case WAYPOINT:
	{
		EnemySpeedType current_speed_type = NONE_SPEED_TYPE;

		switch (enemy->current_point)
		{
		case END_POINT:
			current_speed_type = enemy->spawn_data.end_speed_type;
			break;
		case WAY1_POINT:
			current_speed_type = enemy->spawn_data.way1_speed_type;
			break;
		case WAY2_POINT:
			current_speed_type = enemy->spawn_data.way2_speed_type;
			break;
		}

		switch (current_speed_type)
		{
		case LINEAR:
		{
			vec2 destination = ZeroVector;
			float speed = 0.0f;

			switch (enemy->current_point)
			{
			case END_POINT:
				destination = enemy->spawn_data.end_position;
				speed = enemy->spawn_data.end_speed;
				break;

			case WAY1_POINT:
				destination = enemy->spawn_data.way1_position;
				speed = enemy->spawn_data.way1_speed;
				break;

			case WAY2_POINT:
				destination = enemy->spawn_data.way2_position;
				speed = enemy->spawn_data.way2_speed;
				break;
			}

			vec2 difference = SubVector2(&destination, &enemy->position);
			vec2 direction = GetNormalizedVector2(&difference);
			vec2 transition = ScalarMulVector2Each(&direction, speed * 2 * DeltaTime(), speed * DeltaTime());

			float remain_distance = GetVecter2Length(&difference);

			if (remain_distance < 0.1f)
			{
				switch (enemy->current_point)
				{
				case END_POINT:
					enemy->position = destination;
					DestroyEnemy(enemy);
					break;

				case WAY1_POINT:
				case WAY2_POINT:
					enemy->position = destination;
					enemy->previous_position = destination;
					enemy->way_point_timer = 0.0f;
					enemy->on_waiting = TRUE;
					break;
				}
			}
			else
			{
				enemy->position = AddVector2(&enemy->position, &transition);
			}
		}
			break;

		case EASE_IN:
		{
			vec2 destination = ZeroVector;
			float speed = 0.0f;

			switch (enemy->current_point)
			{
			case END_POINT:
				destination = enemy->spawn_data.end_position;
				speed = enemy->spawn_data.end_speed;
				break;

			case WAY1_POINT:
				destination = enemy->spawn_data.way1_position;
				speed = enemy->spawn_data.way1_speed;
				break;

			case WAY2_POINT:
				destination = enemy->spawn_data.way2_position;
				speed = enemy->spawn_data.way2_speed;
				break;
			}

			vec2 difference = SubVector2(&destination, &enemy->position);
			vec2 direction = GetNormalizedVector2(&difference);
			vec2 entire_difference = SubVector2(&destination, &enemy->previous_position);

			float entire_distance = GetVecter2Length(&entire_difference);
			float remain_distance = GetVecter2Length(&difference);

			float ratio = remain_distance / entire_distance;
			float ease = 3 * (1.0f - ratio) * (1.0f - ratio) + 0.5f;			

			vec2 transition = ScalarMulVector2Each(&direction, ease * speed * 2 * DeltaTime(), ease * speed * DeltaTime());

			if (remain_distance < 0.1f)
			{
				switch (enemy->current_point)
				{
				case END_POINT:
					enemy->position = destination;
					DestroyEnemy(enemy);
					break;

				case WAY1_POINT:
				case WAY2_POINT:
					enemy->position = destination;
					enemy->previous_position = destination;
					enemy->way_point_timer = 0.0f;
					enemy->on_waiting = TRUE;
					break;
				}
			}
			else
			{
				enemy->position = AddVector2(&enemy->position, &transition);
			}
		}
			break;

		case EASE_OUT:
		{
			vec2 destination = ZeroVector;
			float speed = 0.0f;

			switch (enemy->current_point)
			{
			case END_POINT:
				destination = enemy->spawn_data.end_position;
				speed = enemy->spawn_data.end_speed;
				break;

			case WAY1_POINT:
				destination = enemy->spawn_data.way1_position;
				speed = enemy->spawn_data.way1_speed;
				break;

			case WAY2_POINT:
				destination = enemy->spawn_data.way2_position;
				speed = enemy->spawn_data.way2_speed;
				break;
			}

			vec2 difference = SubVector2(&destination, &enemy->position);
			vec2 direction = GetNormalizedVector2(&difference);
			vec2 entire_difference = SubVector2(&destination, &enemy->previous_position);

			float entire_distance = GetVecter2Length(&entire_difference);
			float remain_distance = GetVecter2Length(&difference);

			float ratio = remain_distance / entire_distance;
			float ease = 3 * ratio * ratio + 0.5f;

			vec2 transition = ScalarMulVector2Each(&direction, ease * speed * 2 * DeltaTime(), ease * speed * DeltaTime());

			if (remain_distance < 0.1f)
			{
				switch (enemy->current_point)
				{
				case END_POINT:
					enemy->position = destination;
					DestroyEnemy(enemy);
					break;

				case WAY1_POINT:
				case WAY2_POINT:
					enemy->position = destination;
					enemy->previous_position = destination;
					enemy->way_point_timer = 0.0f;
					enemy->on_waiting = TRUE;
					break;
				}
			}
			else
			{
				enemy->position = AddVector2(&enemy->position, &transition);
			}
		}
			break;

		default:
			break;
		}
	}
	break;

	case WAVE:
	{
		switch (enemy->spawn_data.end_speed_type)
		{
		case LINEAR:
		{
			vec2 difference = SubVector2(&enemy->spawn_data.end_position, &enemy->position);

			float remain_distance = GetVecter2Length(&difference);

			vec2 direction = GetNormalizedVector2(&difference);

			float move_distance = enemy->spawn_data.end_speed * DeltaTime();

			if (move_distance >= remain_distance)
			{
				enemy->position = enemy->spawn_data.end_position;
				DestroyEnemy(enemy);
			}
			else
			{
				vec2 transition = ScalarMulVector2(&direction, move_distance);
				enemy->position = AddVector2(&enemy->position, &transition);
			}
		}
		break;
		}
	}
	break;

	default:
		break;
	}
}