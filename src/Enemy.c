#include "Enemy.h"

#include <stdlib.h>

#include "ConsoleRenderer.h"
#include "Global.h"
#include "Types.h"
#include "Bullet.h"
#include "Vector2.h"
#include "UnionList.h"
#include "MyTime.h"
#include "SwordfishII.h"

static List* enemy_bullet_list = NULL;
static SwordfishII* enemy_swordfish = NULL;
static float fire_timer = 0.0f;

void CreateEnemy(Enemy* enemy)
{
	enemy->id = GenerateID();
	vec2 pos = { (float)(rand() % ScreenWidth() / 2 + 60), (float)(rand() % ScreenHeight())};
	enemy->position = pos;
	enemy->collider.radius = 2.0f;
	enemy->hp = 5;
	enemy->is_destroyed = FALSE;
	wmemcpy_s(enemy->shape, 2, L"бс", 2);
}

void UpdateEnemy(Enemy* enemy)
{
	if (enemy->hp < 0)
	{
		DestroyEnemy(enemy);
	}

	fire_timer += DeltaTime();
	if (fire_timer >= 2.0f)
	{
		Bullet bullet;
		CreateEnemyBullet(&bullet, enemy);
		bullet.direction = MakeDirectionVector2(&enemy_swordfish->position, &bullet.position);

		Insert(enemy_bullet_list, &bullet, sizeof(Bullet));

		fire_timer -= 2.0f;
	}
}

void RenderEnemy(Enemy* enemy)
{
	ScreenDrawString((int)enemy->position.x, (int)enemy->position.y, enemy->shape, FG_YELLOW);
}

void DeleteEnemy(Enemy* enemy)
{
	
}

void SetEnemyBulletList(List* bullet_list)
{
	enemy_bullet_list = bullet_list;
}

void SetSwordFish(SwordfishII* swordfish)
{
	enemy_swordfish = swordfish;
}

void EnemyTakeDamage(Enemy* enemy, int damage)
{
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