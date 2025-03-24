#include "PlayScene.h"

#include <wchar.h>
#include <stdlib.h>

#include "UnionList.h"
#include "ConsoleRenderer.h"
#include "SwordfishII.h"
#include "Types.h"
#include "Enemy.h"
#include "CircleCollider.h"
#include "DebugUtility.h"
#include "Collision.h"

static SwordfishII* swordfish = NULL;
static List* bullet_list = NULL;
static List* enemy_list = NULL;
static List* enemy_bullet_list = NULL;

static void UpdateBulletList();
static void UpdateEnemyList();
static void UpdateEnemyBulletList();

static void RenderBulletList();
static void RenderEnemyList();
static void RenderEnemyBulletList();

void InitializePlayScene()
{
	swordfish = CreateSwordfishII();
	bullet_list = CreateList(BULLET);
	SetBulletList(bullet_list);

	SetSwordFish(swordfish);

	enemy_bullet_list = CreateList(BULLET);
	SetEnemyBulletList(enemy_bullet_list);

	enemy_list = CreateList(ENEMY);
	for (int i = 0; i < 20; ++i)
	{
		Enemy enemy;
		CreateEnemy(&enemy);
		Insert(enemy_list, &enemy, sizeof(Enemy));
	}
}

void UpdatePlayScene()
{
	CheckBulletsToEnemiesCollision(bullet_list, enemy_list);
	CheckSwordfishToEnemyBulletsCollision(swordfish, enemy_bullet_list);

	UpdateSwordfishII(swordfish);
	UpdateBulletList();
	UpdateEnemyBulletList();
	UpdateEnemyList();
}

void RenderPlayScene()
{
	ScreenDrawString(ScreenWidth() / 2 - 2, ScreenHeight() / 2 - 1, L"play", FG_RED);

	RenderSwordfishII(swordfish);
	RenderBulletList();
	RenderEnemyBulletList();
	RenderEnemyList();

	ScreenDrawString(ScreenWidth() / 2 - 8, ScreenHeight() / 2 + 5, L"press A to fire", FG_WHITE);
	ScreenDrawString(ScreenWidth() / 2 - 12, ScreenHeight() / 2 + 9, L"press space to countinue", FG_PINK);

	//ScreenDrawString(0, 0, L"   ▁▁▁", FG_RED);
	//ScreenDrawString(0, 1, L"    █", FG_RED);
	//ScreenDrawString(0, 2, L"  ▅▅█▅▃▂▁", FG_RED);
	//ScreenDrawString(0, 2, L"▅", FG_SKY);
	//ScreenDrawString(1, 2, L"▅", FG_GRAY);
	//ScreenDrawString(0, 3, L"    █", FG_RED);
	//ScreenDrawString(0, 4, L"   ▔▔▔", FG_RED);
}

void ReleasePlayScene()
{
	Node* current_node = NULL;
	if (bullet_list != NULL)
	{
		current_node = bullet_list->head;
		while (current_node != NULL)
		{
			DeleteBullet(&current_node->data.bullet);

			current_node = current_node->next;
		}

		DeleteList(bullet_list);

		bullet_list = NULL;
	}

	if (enemy_bullet_list != NULL)
	{
		current_node = enemy_bullet_list->head;
		while (current_node != NULL)
		{
			DeleteBullet(&current_node->data.bullet);

			current_node = current_node->next;
		}

		DeleteList(enemy_bullet_list);

		enemy_bullet_list = NULL;
		SetEnemyBulletList(NULL);
	}

	DeleteSwordfishII(&swordfish);

	if (enemy_list != NULL)
	{
		current_node = enemy_list->head;
		while (current_node != NULL)
		{
			DeleteEnemy(&current_node->data.enemy);

			current_node = current_node->next;
		}

		DeleteList(enemy_list);

		enemy_list = NULL;
	}
}

static void UpdateBulletList()
{
	Node* previous_node = NULL;
	Node* current_node = bullet_list->head;
	while (current_node != NULL)
	{
		UpdateBullet(&current_node->data.bullet);
		if (IsBulletDestroyed(&current_node->data.bullet))
		{
			DeleteBullet(&current_node->data.bullet);
			current_node = RemoveNode(bullet_list, previous_node, current_node);
		}
		else
		{
			previous_node = current_node;
			current_node = current_node->next;
		}
	}
}

static void UpdateEnemyBulletList()
{
	Node* previous_node = NULL;
	Node* current_node = enemy_bullet_list->head;
	while (current_node != NULL)
	{
		UpdateBullet(&current_node->data.bullet);
		if (IsBulletDestroyed(&current_node->data.bullet))
		{
			DeleteBullet(&current_node->data.bullet);
			current_node = RemoveNode(enemy_bullet_list, previous_node, current_node);
		}
		else
		{
			previous_node = current_node;
			current_node = current_node->next;
		}
	}
}

static void UpdateEnemyList()
{
	Node* previous_node = NULL;
	Node* current_node = enemy_list->head;
	while (current_node != NULL)
	{
		UpdateEnemy(&current_node->data.enemy);
		if (IsEnemyDestroyed(&current_node->data.enemy))
		{
			DeleteEnemy(&current_node->data.enemy);
			current_node = RemoveNode(enemy_list, previous_node, current_node);
		}
		else
		{
			previous_node = current_node;
			current_node = current_node->next;
		}
	}
}

static void RenderBulletList()
{
	Node* current_node = bullet_list->head;
	while (current_node != NULL)
	{
		RenderBullet(&current_node->data.bullet);

		current_node = current_node->next;
	}
}

static void RenderEnemyBulletList()
{
	Node* current_node = enemy_bullet_list->head;
	while (current_node != NULL)
	{
		RenderBullet(&current_node->data.bullet);

		current_node = current_node->next;
	}
}

static void RenderEnemyList()
{
	Node* current_node = enemy_list->head;
	while (current_node != NULL)
	{
		RenderEnemy(&current_node->data.enemy);

		current_node = current_node->next;
	}
}