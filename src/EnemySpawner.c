#include "EnemySpawner.h"

#include <math.h>

#include "FileLoader.h"
#include "UnionList.h"

// ���� ���� ����
// 
// ����(ȭ�� ������ ��->���� ��/���� �� <-> �Ʒ��� ��, �̵� �ӵ� ������/������)
// ������ �̵�(�̵� ����Ʈn��, �� ���� ���ð�, ���� �̵� �ӵ� ������/������)
// ������ ������ �̵�(�� �� ����, �ٸ� �� Ŀ���� �۾�����)
//
// ���� ���� ����

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