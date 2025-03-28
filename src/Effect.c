#include "Effect.h"

#include <stdlib.h>
#include <wchar.h>

#include "UnionList.h"
#include "Vector2.h"
#include "FileLoader.h"
#include "DebugUtility.h"
#include "Global.h"
#include "MyTime.h"
#include "TypeDefines.h"
#include "ConsoleRenderer.h"

static EffectData* effect_data[EFFECT_MAX] = { 0 };

static void CreateEffectData(const wchar_t* file_name, EffectName name);
static void ReleaseEffectDataList(EffectData* data);

static WORD RandomAllColor();
static WORD RandomRedYellow();

void InitializeEffectData()
{
	CreateEffectData(L"effect_canon_hit.txt", effect_canon_hit);
	//CreateEffectData(L"effect_machingun_hit.txt", MACHINEGUN_HIT_EFFECT);
	CreateEffectData(L"effect_player_flame.txt", effect_player_flame);
	CreateEffectData(L"effect_canon_flame.txt", effect_canon_flame);
	CreateEffectData(L"effect_enemy_destroy.txt", effect_enemy_destroy);
	CreateEffectData(L"effect_boss_destroy.txt", effect_boss_destroy);
}

static void CreateEffectData(const wchar_t* file_name, EffectName name)
{
	StringData* sd = LoadSingleLineData(file_name);
	if (sd == NULL)
	{
		return;
	}

	EffectData* data = (EffectData*)malloc(sizeof(EffectData));
	if (data == NULL)
	{
		return;
	}

	effect_data[name] = data;

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
	free(sd);
}

void CreateEffect(Effect* effect, const Vector2* position, EffectName name)
{
	effect->id = GenerateID();
	effect->timer = 0.0f;
	effect->is_destroyed = FALSE;
	effect->position = *position;
	effect->name = name;
}

void UpdateEffect(Effect* effect)
{
	EffectData* data = effect_data[effect->name];

	effect->timer += DeltaTime();
	if (effect->timer >= data->duration)
	{
		effect->is_destroyed = TRUE;
	}
}

void RenderEffect(Effect* effect)
{
	EffectData* data = effect_data[effect->name];

	int frame = (int)(effect->timer / data->duration * data->frames);

	List* frame_list = data->effect_frame_list;

	Node* current_frame_list = frame_list->head;
	while (frame--)
	{
		current_frame_list = current_frame_list->next;
	}

	Node* current_shape_node = current_frame_list->data.effect_list->head;
	while (current_shape_node != NULL)
	{
		wchar_t shape = current_shape_node->data.draw_unit.shape;
		WORD attribute = current_shape_node->data.draw_unit.attribute;
		Vector2 position = AddVector2(&effect->position, &current_shape_node->data.draw_unit.position);

		switch (attribute)
		{
		case 99:
			attribute = RandomAllColor();
			break;

		case 98:
			attribute = RandomRedYellow();
			break;

		default:
			break;
		}

		ScreenDrawChar((int)position.x, (int)position.y, shape, attribute);

		current_shape_node = current_shape_node->next;
	}
}

BOOL IsEffectDestroyed(Effect* effect)
{
	return effect->is_destroyed;
}

void ReleaseEffectData()
{
	for (int i = 0; i < EFFECT_MAX; ++i)
	{
		ReleaseEffectDataList(effect_data[i]);
		effect_data[i] = NULL;
	}
}

static void ReleaseEffectDataList(EffectData* data)
{
	if (data != NULL)
	{
		Node* current_node = data->effect_frame_list->head;
		while (current_node != NULL)
		{
			DeleteList(current_node->data.effect_list);

			current_node = current_node->next;
		}

		DeleteList(data->effect_frame_list);

		free(data);
	}
}

static WORD RandomAllColor()
{
	return rand() % 15 + 1;
}

static WORD RandomRedYellow()
{
	return rand() % 2 == 0 ? FG_RED : FG_YELLOW;
}