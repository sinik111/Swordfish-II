#include "Collision.h"

#include "UnionList.h"
#include "TypeDefines.h"
#include "Enemy.h"
#include "DebugUtility.h"
#include "Bullet.h"
#include "Player.h"
#include "Boss.h"

BOOL IsCollide(const CircleCollider* a, const CircleCollider* b)
{
	vec2 sub_vec = SubVector2(&a->position, &b->position);
	float distance = GetVecter2Length(&sub_vec);

	if (a->radius + b->radius >= distance)
	{
		return TRUE;
	}

	return FALSE;
}

void CheckBulletsToBossCollision(List* bullet_list, Boss* boss)
{
	Node* previous_bullet_node = NULL;
	Node* current_bullet_node = bullet_list->head;
	while (current_bullet_node != NULL)
	{
		if (IsCollide(&current_bullet_node->data.bullet.collider, &boss->collider))
		{
			PlayBulletHitEffect(&current_bullet_node->data.bullet);
			DestroyBullet(&current_bullet_node->data.bullet);
			BossTakeDamage(boss, current_bullet_node->data.bullet.damage);
		}

		previous_bullet_node = current_bullet_node;
		current_bullet_node = current_bullet_node->next;
	}
}

void CheckBulletsToEnemiesCollision(List* bullet_list, List* enemy_list)
{
	Node* previous_bullet_node = NULL;
	Node* current_bullet_node = bullet_list->head;
	while (current_bullet_node != NULL)
	{
		Node* previous_enemy_node = NULL;
		Node* current_enemy_node = enemy_list->head;
		while (current_enemy_node != NULL)
		{
			if (IsCollide(&current_bullet_node->data.bullet.collider, &current_enemy_node->data.enemy.collider))
			{
				//DebugLog("hit\n");
				PlayBulletHitEffect(&current_bullet_node->data.bullet);
				DestroyBullet(&current_bullet_node->data.bullet);
				EnemyTakeDamage(&current_enemy_node->data.enemy, current_bullet_node->data.bullet.damage);
			}

			previous_enemy_node = current_enemy_node;
			current_enemy_node = current_enemy_node->next;
		}

		previous_bullet_node = current_bullet_node;
		current_bullet_node = current_bullet_node->next;
	}
}

void CheckPlayerToEnemyBulletsCollision(Player* player, List* enemy_bullet_list)
{
	Node* previous_bullet_node = NULL;
	Node* current_bullet_node = enemy_bullet_list->head;
	while (current_bullet_node != NULL)
	{
		if (IsCollide(&player->collider, &current_bullet_node->data.bullet.collider))
		{
			//DebugLog("hit\n");
			DestroyBullet(&current_bullet_node->data.bullet);
			PlayerTakeDamage(player, current_bullet_node->data.bullet.damage);
		}

		previous_bullet_node = current_bullet_node;
		current_bullet_node = current_bullet_node->next;
	}
}