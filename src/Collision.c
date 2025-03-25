#include "Collision.h"

#include "UnionList.h"
#include "Types.h"
#include "Enemy.h"
#include "DebugUtility.h"
#include "Bullet.h"
#include "Player.h"

void CheckBulletsToEnemyCollision(List* bullet_list, Enemy* enemy)
{
	Node* previous_bullet_node = NULL;
	Node* current_bullet_node = bullet_list->head;
	while (current_bullet_node != NULL)
	{
		if (IsCollide(&current_bullet_node->data.bullet.collider, &enemy->collider))
		{
			DebugLog("hit\n");
			
			DestroyBullet(&current_bullet_node->data.bullet);
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
				DebugLog("hit\n");
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

void CheckplayerToEnemyBulletsCollision(Player* player, List* enemy_bullet_list)
{
	Node* previous_bullet_node = NULL;
	Node* current_bullet_node = enemy_bullet_list->head;
	while (current_bullet_node != NULL)
	{
		if (IsCollide(&player->collider, &current_bullet_node->data.bullet.collider))
		{
			DebugLog("hit\n");
			DestroyBullet(&current_bullet_node->data.bullet);
			PlayerTakeDamage(player, current_bullet_node->data.bullet.damage);
		}

		previous_bullet_node = current_bullet_node;
		current_bullet_node = current_bullet_node->next;
	}
}