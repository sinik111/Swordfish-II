#include "PlayScene.h"

#include <wchar.h>
#include <stdlib.h>

#include "UnionList.h"
#include "ConsoleRenderer.h"
#include "Player.h"
#include "TypeDefines.h"
#include "Enemy.h"
#include "Collision.h"
#include "DebugUtility.h"
#include "UIplayerHP.h"
#include "Effect.h"
#include "MyTime.h"
#include "Game.h"
#include "Boss.h"
#include "Item.h"

static Player* player = NULL;
static List* bullet_list = NULL;
static List* enemy_list = NULL;
static List* enemy_bullet_list = NULL;
static UIplayerHP* hp_ui = NULL;
static List* effect_list = NULL;
static Boss* boss = NULL;
static List* item_list = NULL;

static float item_timer = 0.0f;
static float item_rate = 3.0f;

static void UpdateBulletList();
static void UpdateEnemyList();
static void UpdateEnemyBulletList();
static void UpdateEffectList();
static void UpdateItemList();

static void RenderBulletList();
static void RenderEnemyList();
static void RenderEnemyBulletList();
static void RenderEffectList();
static void RenderItemList();

void InitializePlayScene()
{
	InitializeShapeData();

	player = CreatePlayer();
	bullet_list = CreateList(BULLET);
	enemy_bullet_list = CreateList(BULLET);
	enemy_list = CreateList(ENEMY);

	hp_ui = CreateUIplayerHP();

	InitializeEffectData();

	effect_list = CreateList(EFFECT);

	InitializeEnemySpawnData();

	boss = CreateBoss();

	item_list = CreateList(ITEM);

	UpdateTime();
}

void UpdatePlayScene()
{
	// Boss Encounter
	if (IsSpawnerEmpty() && IsEnemyAllDestroyed())
	{
		// boss 생성코드
		//ChangeScene(END);
	}

	// collision
	CheckBulletsToEnemiesCollision(bullet_list, enemy_list);
	CheckPlayerToEnemyBulletsCollision(player, enemy_bullet_list);
	CheckPlayerToItemsCollision(player, item_list);

	if (boss)
	{
		CheckBulletsToBossCollision(bullet_list, boss);
	}

	// object
	UpdatePlayer(player);
	UpdateBulletList();
	UpdateEnemyBulletList();
	UpdateEnemyList();

	if (boss)
	{
		UpdateBoss(boss);
	}

	UpdateEnemySpawner();
	UpdateItemList();

	// effect
	UpdateEffectList();

	// ui
	UpdateUIplayerHP(hp_ui);
}

void RenderPlayScene()
{
	ScreenDrawString(ScreenWidth() / 2 - 2, ScreenHeight() / 2 - 1, L"play", FG_RED);

	// object
	RenderPlayer(player);
	RenderBulletList();
	RenderEnemyBulletList();
	RenderEnemyList();
	RenderBoss(boss);
	RenderItemList();

	// effect
	RenderEffectList();

	// ui
	RenderUIplayerHP(hp_ui);

	ScreenDrawString(ScreenWidth() / 2 - 8, ScreenHeight() / 2 + 5, L"press A to fire", FG_WHITE);
	ScreenDrawString(ScreenWidth() / 2 - 12, ScreenHeight() / 2 + 9, L"press space to countinue", FG_PINK);
}

void ReleasePlayScene()
{
	Node* current_node = NULL;
	if (bullet_list != NULL)
	{
		current_node = bullet_list->head;
		while (current_node != NULL)
		{
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
			current_node = current_node->next;
		}

		DeleteList(enemy_bullet_list);

		enemy_bullet_list = NULL;
	}

	DeletePlayer(&player);

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

	if (hp_ui != NULL)
	{
		free(hp_ui);
	}

	DeleteList(effect_list);
	effect_list = NULL;

	ReleaseEffectData();
	ReleaseShapeData();
	ReleaseEnemySpawnData();

	free(boss);
	boss = NULL;

	DeleteList(item_list);
	item_list = NULL;
}

List* GetEffectList()
{
	return effect_list;
}

List* GetPlayerBulletList()
{
	return bullet_list;
}

List* GetEnemyList()
{
	return enemy_list;
}

List* GetEnemyBulletList()
{
	return enemy_bullet_list;
}

Player* GetPlayer()
{
	return player;
}

BOOL IsEnemyAllDestroyed()
{
	if (enemy_list->head == NULL)
	{
		return TRUE;
	}

	return FALSE;
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

static void UpdateEffectList()
{
	Node* previous_node = NULL;
	Node* current_node = effect_list->head;
	while (current_node != NULL)
	{
		UpdateEffect(&current_node->data.effect);
		if (IsEffectDestroyed(&current_node->data.effect))
		{
			current_node = RemoveNode(effect_list, previous_node, current_node);
		}
		else
		{
			previous_node = current_node;
			current_node = current_node->next;
		}
	}
}

static void UpdateItemList()
{
	item_timer += DeltaTime();
	if (item_timer >= item_rate)
	{
		item_timer -= item_rate;

		Item item;
		CreateItem(&item);

		Insert(item_list, &item, sizeof(Item));
	}

	Node* previous_node = NULL;
	Node* current_node = item_list->head;
	while (current_node != NULL)
	{
		UpdateItem(&current_node->data.item);
		if (IsItemDestroyed(&current_node->data.item))
		{
			current_node = RemoveNode(item_list, previous_node, current_node);
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

static void RenderEffectList()
{
	Node* current_node = effect_list->head;
	while (current_node != NULL)
	{
		RenderEffect(&current_node->data.effect);

		current_node = current_node->next;
	}
}

static void RenderItemList()
{
	Node* previous_node = NULL;
	Node* current_node = item_list->head;
	while (current_node != NULL)
	{
		RenderItem(&current_node->data.item);

		current_node = current_node->next;
	}
}