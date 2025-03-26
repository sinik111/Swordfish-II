#include "EnemySpawner.h"

#include <stdlib.h>

#include "FileLoader.h"
#include "UnionList.h"
#include "MyTime.h"
#include "PlayScene.h"
#include "Enemy.h"
#include "Global.h"
#include "DebugUtility.h"
#include "TypeDefines.h"

// 시작 지점 지정
// 
// 직진(화면 오른쪽 끝->왼쪽 끝/위쪽 끝 <-> 아래쪽 끝, 이동 속도 선형적/비선형적)
// 다지점 이동(이동 포인트n개, 각 지점 대기시간, 지점 이동 속도 선형적/비선형적)
// 오르락 내리락 이동(한 축 직진, 다른 축 커졌다 작아졌다)
//
// 종료 지점 지정

static List* enemy_spawn_data_list = NULL;
static float enemy_spawn_timer;

static void CreateEnemySpawnData(const wchar_t* file_name);

void InitializeEnemySpawnData()
{
	enemy_spawn_timer = 0.0f;

	CreateEnemySpawnData(L"enemy_spawn_data.txt");
}

static void CreateEnemySpawnData(const wchar_t* file_name)
{
	StringData* sd = LoadMultiLineData(file_name);
	if (sd == NULL)
	{
		return;
	}

	enemy_spawn_data_list = CreateList(ENEMY_SPAWN_DATA);

	const wchar_t* cursor = sd->data;
	int character_count = 0;

	for (int i = 0; i < sd->count; ++i)
	{
		EnemySpawnData data;
		data.id = GenerateID();
		int result = swscanf(cursor,
			L"%d %d %hd %f %f %hd %hd %d %f %f %hd %hd %d %f %f %hd %hd %d %f %f%n",
			&data.enemy_type, &data.enemy_movement_type, &data.spawn_time, &data.spawn_position.x,
			&data.spawn_position.y, &data.way_count, &data.way1_speed, &data.way1_speed_type,
			&data.way1_position.x, &data.way1_position.y, &data.way1_time, &data.way2_speed,
			&data.way2_speed_type, &data.way2_position.x, &data.way2_position.y, &data.way2_time,
			&data.end_speed, &data.end_speed_type, &data.end_position.x, &data.end_position.y,
			&character_count);

		Insert(enemy_spawn_data_list, &data, sizeof(EnemySpawnData));

		cursor += character_count;
	}

	free(sd->data);
	free(sd);
}

void UpdateEnemySpawner()
{
	enemy_spawn_timer += DeltaTime();

	Node* fastest_node = enemy_spawn_data_list->head;
	while (fastest_node != NULL)
	{
		if (fastest_node->data.spawn_data.spawn_time <= enemy_spawn_timer)
		{
			Enemy enemy;
			enemy.spawn_data = fastest_node->data.spawn_data;
			CreateEnemy(&enemy);

			Insert(GetEnemyList(), &enemy, sizeof(Enemy));
			DebugLog("enemy created\n");
			fastest_node = Remove(enemy_spawn_data_list, fastest_node->data.id);
		}
		else
		{
			break;
		}
	}
}

void ReleaseEnemySpawnData()
{
	if (enemy_spawn_data_list != NULL)
	{
		DeleteList(enemy_spawn_data_list);

		enemy_spawn_data_list = NULL;
	}
}

BOOL IsSpawnerEmpty()
{
	if (enemy_spawn_data_list->head == NULL)
	{
		return TRUE;
	}
	
	return FALSE;
}