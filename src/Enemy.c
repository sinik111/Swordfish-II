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

static float fire_timer = 0.0f;

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
	//wmemcpy_s(enemy->shape, 2, L"бс", 2);
}

void UpdateEnemy(Enemy* enemy)
{
	vec2 direction = SubVector2(&enemy->spawn_data.end_position, &enemy->position);
	float remain_distance = GetVecter2Length(&direction);
	NormalizeVector2(&direction);
	float movement = enemy->spawn_data.end_speed * DeltaTime();

	if (movement >= remain_distance)
	{
		enemy->position = enemy->spawn_data.end_position;
		DestroyEnemy(enemy);
	}
	else
	{
		vec2 transition = ScalarMulVector2(&direction, movement);
		enemy->position = AddVector2(&enemy->position, &transition);
	}

	if (enemy->hp < 0)
	{
		DestroyEnemy(enemy);
	}

	fire_timer += DeltaTime();
	if (fire_timer >= 2.0f)
	{
		Bullet bullet;
		CreateEnemyBullet(&bullet, enemy);
		bullet.direction = LeftVector;//MakeDirectionVector2(&enemy_player->position, &bullet.position);

		Insert(GetEnemyBulletList(), &bullet, sizeof(Bullet));

		fire_timer -= 2.0f;
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
}

BOOL IsEnemyDestroyed(Enemy* enemy)
{
	return enemy->is_destroyed;
}