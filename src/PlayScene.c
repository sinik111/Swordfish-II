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
#include "PlayBackground.h"
#include "Input.h"
#include "UIBossHp.h"
#include "SoundController.h"

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
static UIBossHP* boss_hp_ui= NULL;

static float item_timer = 0.0f;
static float item_rate = 3.0f;
static int score = 0;

static float key_notice_timer = 5.0f;

static float back_to_intro_timer = 30.0f;

static BOOL end_music_on = FALSE;

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

	InitializePlayBackgroundData();

	player = CreatePlayer();
	hp_ui = CreateUIPlayerHP();
	
	skill_ui = CreateUIPlayerSkill();

	bullet_list = CreateList(BULLET);
	enemy_bullet_list = CreateList(BULLET);
	enemy_list = CreateList(ENEMY);
	effect_list = CreateList(EFFECT);
	item_list = CreateList(ITEM);

	item_timer = 0.0f;
	back_to_intro_timer = 30.0f;
	key_notice_timer = 5.0f;

	score = 0;

	end_music_on = FALSE;

	PlayGameSound(play_music);

	SetGameSoundVolume(play_music, 0.05f);

	UpdateTime();
}

void UpdatePlayScene()
{
	// Boss Encounter
	if (IsSpawnerEmpty() && IsEnemyAllDestroyed() && boss == NULL)
	{
		// boss 생성코드
		boss = CreateBoss();
		boss_hp_ui = CreateUIBossHP();
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

	// background
	UpdatePlayBackground();

	// object
	UpdatePlayer(player);
	UpdateBulletList();
	UpdateEnemyBulletList();
	UpdateEnemyList();

	if (boss != NULL)
	{
		UpdateBoss(boss);
		UpdateUIBossHP(boss_hp_ui);
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
	// background
	RenderPlayBackground();

	// object
	RenderPlayer(player);
	RenderBulletList();
	RenderEnemyBulletList();
	RenderEnemyList();
	if (boss != NULL)
	{
		RenderBoss(boss);
		RenderUIBossHP(boss_hp_ui);
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

	int score_calc = score;
	int score100000 = score_calc / 100000;
	score_calc %= 100000;
	int score10000 = score_calc / 10000;
	score_calc %= 10000;
	int score1000 = score_calc / 1000;
	score_calc %= 1000;
	int score100 = score_calc / 100;
	score_calc %= 100;
	int score10 = score_calc / 10;
	score_calc %= 10;
	int score1 = score_calc;

	wchar_t score_buffer[20] = { 0 };

	swprintf(score_buffer, 20, L"SCORE %d%d%d%d%d%d", score100000, score10000, score1000, score100, score10, score1);

	ScreenDrawString(50, 0, score_buffer, FG_WHITE);

	if (key_notice_timer > 0)
	{
		key_notice_timer -= DeltaTime();

		ScreenDrawString(51, 20, L"press ← ↑ ↓ → to move", FG_WHITE);

		ScreenDrawString(54, 22, L"press A to fire", FG_WHITE);
	}

	if (IsSpawnerEmpty() && IsEnemyAllDestroyed() && boss != NULL)
	{
		if (boss->is_destroyed)
		{
			if (!end_music_on)
			{
				end_music_on = TRUE;

				StopGameSound(play_music);

				PlayGameSound(end_music);

				SetGameSoundVolume(end_music, 0.05f);
			}

			ClearList(enemy_bullet_list);

			back_to_intro_timer -= DeltaTime();
			if (back_to_intro_timer < 0 || IsKeyReleased(VK_SPACE))
			{
				ChangeScene(MENU);

				return;
			}

			ScreenDrawString(57, 14, L"VICTORY", FG_WHITE);

			wchar_t end_buffer[100] = { 0 };

			swprintf(end_buffer, 100, L"press SPACE to back to intro...%d", (int)back_to_intro_timer);

			ScreenDrawString(45, 20, end_buffer, FG_WHITE);
		}
	}
	
	if (player->is_destroyed)
	{
		back_to_intro_timer -= DeltaTime();
		if (back_to_intro_timer < 0 || IsKeyReleased(VK_SPACE))
		{
			ChangeScene(MENU);

			return;
		}

		ScreenDrawString(58, 14, L"DEFEAT", FG_WHITE);
		
		wchar_t end_buffer[100] = { 0 };

		swprintf(end_buffer, 100, L"press SPACE to back to intro...%d", (int)back_to_intro_timer);

		ScreenDrawString(45, 20, end_buffer, FG_WHITE);
	}
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

	if (boss_hp_ui != NULL)
	{
		free(boss_hp_ui);
		boss_hp_ui = NULL;
	}

	ReleaseEffectData();
	ReleaseShapeData();
	ReleaseEnemySpawnData();
	ReleasePlayBackgroundData();

	StopGameSound(play_music);
	StopGameSound(end_music);
	StopGameSound(canon_sound);
	StopGameSound(machine_gun_sound);
	StopGameSound(skill_charge_sound);
	StopGameSound(skill_fire_sound);
	StopGameSound(explosion_sound);
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

void AddScore(int add_score)
{
	score += add_score;
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