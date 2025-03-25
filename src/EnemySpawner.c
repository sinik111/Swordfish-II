#include "EnemySpawner.h"

#include <math.h>

#include "FileLoader.h"
#include "UnionList.h"

// 시작 지점 지정
// 
// 직진(화면 오른쪽 끝->왼쪽 끝/위쪽 끝 <-> 아래쪽 끝, 이동 속도 선형적/비선형적)
// 다지점 이동(이동 포인트n개, 각 지점 대기시간, 지점 이동 속도 선형적/비선형적)
// 오르락 내리락 이동(한 축 직진, 다른 축 커졌다 작아졌다)
//
// 종료 지점 지정

List* enemy_spawn_data_list = NULL;

static void CreateEnemySpawnData(const wchar_t* file_name);

void InitializeEnemySpawnData()
{
	CreateEnemySpawnData(L"enemy_spawn_data.txt");
}

static void CreateEnemySpawnData(const wchar_t* file_name)
{
	StringData* sd = LoadMultiLineData(file_name);
	if (sd == NULL)
	{
		return;
	}

	/*enemy_spawn_data_list = CreateList()

	EffectData* data = (EffectData*)malloc(sizeof(EffectData));
	if (data == NULL)
	{
		return;
	}

	switch (type)
	{
	case BULLET_HIT_EFFECT:
		bullet_hit_effect_data = data;
		break;

	case PLAYER_FLAME_EFFECT:
		player_flame_effect_data = data;
		break;

	default:
		return;
	}

	List* effect_frame_list = CreateList(LIST);
	effect_frame_list->id = GenerateID();
	data->id = GenerateID();
	data->effect_frame_list = effect_frame_list;
	data->frames = sd->count;
	data->duration = (float)sd->additional1 / sd->additional3;

	for (int c = 0; c < sd->count; ++c)
	{
		List* effect_frames = CreateList(DRAW_UNIT);
		effect_frames->id = GenerateID();
		Insert(effect_frame_list, &effect_frames, sizeof(List*));

		for (int i = 0; i < sd->n; ++i)
		{
			for (int j = 0; j < sd->m; ++j)
			{
				wchar_t shape = 0;
				shape = *(sd->data + (j + i * sd->m + c * sd->m * sd->n));
				if (shape != L' ')
				{
					DrawUnit draw_unit;
					Vector2 position = { (float)(j - sd->m / 2), (float)(i - sd->n / 2) };
					CreateDrawUnit(&draw_unit, &position, &shape, sd->additional2);
					Insert(effect_frames, &draw_unit, sizeof(DrawUnit));
				}
			}
		}
	}

	free(sd->data);
	free(sd);*/
}