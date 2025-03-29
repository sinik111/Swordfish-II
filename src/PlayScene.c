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
#include "UIPlayerSkill.h"
#include "Beam.h"

static Player* player = NULL;
static List* bullet_list = NULL;
static List* enemy_list = NULL;
static List* enemy_bullet_list = NULL;
static UIPlayerHP* hp_ui = NULL;
static List* effect_list = NULL;
static Boss* boss = NULL;
static List* item_list = NULL;
static UIPlayerSkill* skill_ui = NULL;
static Beam* beam = NULL;

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

static BOOL IsEnemyAllDestroyed();

void InitializePlayScene()
{
	InitializeShapeData();
	InitializeEffectData();
	InitializeEnemySpawnData();

	player = CreatePlayer();
	hp_ui = CreateUIPlayerHP();
	
	skill_ui = CreateUIPlayerSkill();

	bullet_list = CreateList(BULLET);
	enemy_bullet_list = CreateList(BULLET);
	enemy_list = CreateList(ENEMY);
	effect_list = CreateList(EFFECT);
	item_list = CreateList(ITEM);

	UpdateTime();
}

void UpdatePlayScene()
{
	// Boss Encounter
	if (IsSpawnerEmpty() && IsEnemyAllDestroyed() && boss == NULL)
	{
		// boss 생성코드
		boss = CreateBoss();
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

	if (boss != NULL)
	{
		UpdateBoss(boss);
	}

	UpdateEnemySpawner();
	UpdateItemList();

	if (beam != NULL)
	{
		UpdateBeam(beam);
	}

	// effect
	UpdateEffectList();

	// ui
	UpdateUIPlayerHP(hp_ui);
	UpdateUIPlayerSKill(skill_ui);
}

void RenderPlayScene()
{
	// object
	RenderPlayer(player);
	RenderBulletList();
	RenderEnemyBulletList();
	RenderEnemyList();
	if (boss != NULL)
	{
		RenderBoss(boss);
	}
	RenderItemList();
	if (beam != NULL)
	{
		RenderBeam(beam);
	}

	// effect
	RenderEffectList();

	// ui
	RenderUIPlayerHP(hp_ui);
	RenderUIPlayerSKill(skill_ui);
}

void ReleasePlayScene()
{
	if (bullet_list != NULL)
	{
		DeleteList(bullet_list);
		bullet_list = NULL;
	}

	if (enemy_bullet_list != NULL)
	{
		DeleteList(enemy_bullet_list);
		enemy_bullet_list = NULL;
	}

	if (player != NULL)
	{
		free(player);
		player = NULL;
	}

	if (enemy_list != NULL)
	{
		DeleteList(enemy_list);
		enemy_list = NULL;
	}

	if (hp_ui != NULL)
	{
		free(hp_ui);
		hp_ui = NULL;
	}

	if (effect_list != NULL)
	{
		DeleteList(effect_list);
		effect_list = NULL;
	}
	
	if (boss != NULL)
	{
		free(boss);
		boss = NULL;
	}

	if (item_list != NULL)
	{
		DeleteList(item_list);
		item_list = NULL;
	}
	
	if (skill_ui != NULL)
	{
		free(skill_ui);
		skill_ui = NULL;
	}
	
	if (beam != NULL)
	{
		free(beam);
		beam = NULL;
	}

	ReleaseEffectData();
	ReleaseShapeData();
	ReleaseEnemySpawnData();
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

Boss* GetBoss()
{
	return boss;
}

void SetBeam(Beam* new_beam)
{
	beam = new_beam;
}

static BOOL IsEnemyAllDestroyed()
{
	return enemy_list->head == NULL;
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