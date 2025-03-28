#include "Collision.h"

#include "UnionList.h"
#include "TypeDefines.h"
#include "Enemy.h"
#include "DebugUtility.h"
#include "Bullet.h"
#include "Player.h"
#include "Boss.h"

static BOOL IsCollide(const CircleCollider* a, const CircleCollider* b);

static BOOL IsCollide(const CircleCollider* a, const CircleCollider* b)
{
	vec2 sub_vec = SubVector2(&a->position, &b->position);

	return a->radius * a->radius + b->radius * b->radius >= GetVecter2Square(&sub_vec);
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

void CheckPlayerToItemsCollision(Player* player, List* item_list)
{
	Node* previous_item_node = NULL;
	Node* current_item_node = item_list->head;
	while (current_item_node != NULL)
	{
		if (IsCollide(&player->collider, &current_item_node->data.item.collider))
		{
			DestroyItem(&current_item_node->data.item);
			PlayerGetItem(player);
		}

		previous_item_node = current_item_node;
		current_item_node = current_item_node->next;
	}
}