#include "EffectBulletHit.h"

#include <stdlib.h>

#include "UnionList.h"
#include "Vector2.h"
#include "FileLoader.h"
#include "DebugUtility.h"
#include "Global.h"
#include "MyTime.h"
#include "Types.h"
#include "ConsoleRenderer.h"

static EffectBulletHitData* effect_bullet_hit_data = NULL;

void CreateEffectBulletHitData()
{
	StringData* sd = LoadSingleLineData(L"effect_bullet_hit.txt");

	effect_bullet_hit_data = (EffectBulletHitData*)malloc(sizeof(EffectBulletHitData));
	if (effect_bullet_hit_data == NULL)
	{
		return;
	}

	List* effect_frame_list = CreateList(LIST);
	effect_frame_list->id = GenerateID();
	effect_bullet_hit_data->id = GenerateID();
	effect_bullet_hit_data->effect_frame_list = effect_frame_list;
	effect_bullet_hit_data->frames = sd->count;
	effect_bullet_hit_data->duration = (float)sd->additional1;
	
	for (int c = 0; c < sd->count; ++c)
	{
		List* effect_frames = CreateList(EFFECT_UNIT);
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
					EffectUnit effect_unit;
					Vector2 position = { (float)(j - sd->m / 2), (float)(i - sd->n / 2) };
					CreateEffectUnit(&effect_unit, &position, &shape);
					effect_unit.id = GenerateID();
					Insert(effect_frames, &effect_unit, sizeof(EffectUnit));
				}
			}
		}
	}
	
	free(sd->data);
	free(sd);

	int a = 0;
	//for (int i = 0; i < sd->count; ++i)
	//{
	//	int a, b, c, d;

	//	int result = swscanf(sd->data + sd->m * sd->n * i, L"%d %d %d %d", &a, &b, &c, &d);

	//	int asdf = 0;
	//}
}

void CreateEffectBulletHit(EffectBulletHit* effect)
{
	effect->id = GenerateID();
	effect->timer = 0.0f;
	effect->is_destroyed = FALSE;
}

void UpdateEffectBulletHit(EffectBulletHit* effect)
{
	effect->timer += DeltaTime();
	if (effect->timer >= effect_bullet_hit_data->duration)
	{
		effect->is_destroyed = TRUE;
	}
}

void RenderEffectBulletHit(EffectBulletHit* effect)
{
	int frame = (int)(effect->timer / effect_bullet_hit_data->duration * effect_bullet_hit_data->frames);

	List* frame_list = effect_bullet_hit_data->effect_frame_list;
	Node* current_frame_list = frame_list->head;
	while (frame--)
	{
		current_frame_list = current_frame_list->next;
	}

	Node* current_shape_node = current_frame_list->data.effect_list->head;
	while (current_shape_node != NULL)
	{
		Vector2 position = AddVector2(&effect->position, &current_shape_node->data.effect_unit.position);
		ScreenDrawChar((int)position.x, (int)position.y, current_shape_node->data.effect_unit.shape, FG_WHITE);

		current_shape_node = current_shape_node->next;
	}
}

BOOL IsEffectBulletHitDestroyed(EffectBulletHit* effect)
{
	return effect->is_destroyed;
}

void DeleteEffectBulletHit()
{
	if (effect_bullet_hit_data != NULL)
	{
		Node* current_node = effect_bullet_hit_data->effect_frame_list->head;
		while (current_node != NULL)
		{
			DeleteList(current_node->data.effect_list);

			current_node = current_node->next;
		}

		DeleteList(effect_bullet_hit_data->effect_frame_list);

		free(effect_bullet_hit_data);

		effect_bullet_hit_data = NULL;
	}
}