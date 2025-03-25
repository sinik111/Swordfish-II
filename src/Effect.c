#include "Effect.h"

#include <stdlib.h>

#include "UnionList.h"
#include "Vector2.h"
#include "FileLoader.h"
#include "DebugUtility.h"
#include "Global.h"
#include "MyTime.h"
#include "Types.h"
#include "ConsoleRenderer.h"

static EffectData* bullet_hit_effect_data = NULL;
static EffectData* player_flame_effect_data = NULL;

static void CreateEffectData(const wchar_t* file_name, EffectType type);
static void ReleaseEffectDataList(EffectData* data);

void InitializeEffectData()
{
	CreateEffectData(L"effect_bullet_hit.txt", BULLET_HIT_EFFECT);
	CreateEffectData(L"player_flame.txt", PLAYER_FLAME_EFFECT);
}

static void CreateEffectData(const wchar_t* file_name, EffectType type)
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
	free(sd);
}

void CreateEffect(Effect* effect, const Vector2* position, EffectType type)
{
	effect->id = GenerateID();
	effect->timer = 0.0f;
	effect->is_destroyed = FALSE;
	effect->position = *position;
	effect->type = type;
}

void UpdateEffect(Effect* effect)
{
	EffectData* effect_data = NULL;
	switch (effect->type)
	{
	case BULLET_HIT_EFFECT:
		effect_data = bullet_hit_effect_data;
		break;

	case PLAYER_FLAME_EFFECT:
		effect_data = player_flame_effect_data;
		break;

	default:
		return;
	}

	effect->timer += DeltaTime();
	if (effect->timer >= effect_data->duration)
	{
		effect->is_destroyed = TRUE;
	}
}

void RenderEffect(Effect* effect)
{
	EffectData* effect_data = NULL;
	switch (effect->type)
	{
	case BULLET_HIT_EFFECT:
		effect_data = bullet_hit_effect_data;
		break;

	case PLAYER_FLAME_EFFECT:
		effect_data = player_flame_effect_data;
		break;

	default:
		return;
	}

	int frame = (int)(effect->timer / effect_data->duration * effect_data->frames);

	List* frame_list = effect_data->effect_frame_list;

	Node* current_frame_list = frame_list->head;
	while (frame--)
	{
		current_frame_list = current_frame_list->next;
	}

	Node* current_shape_node = current_frame_list->data.effect_list->head;
	while (current_shape_node != NULL)
	{
		Vector2 position = AddVector2(&effect->position, &current_shape_node->data.draw_unit.position);
		WORD attribute = current_shape_node->data.draw_unit.attribute;
		if (attribute == 99)
		{
			attribute = rand() % 15 + 1;
		}
		ScreenDrawChar((int)position.x, (int)position.y, current_shape_node->data.draw_unit.shape, attribute);

		current_shape_node = current_shape_node->next;
	}
}

BOOL IsEffectDestroyed(Effect* effect)
{
	return effect->is_destroyed;
}

void ReleaseEffectData()
{
	ReleaseEffectDataList(bullet_hit_effect_data);
	bullet_hit_effect_data = NULL;
	ReleaseEffectDataList(player_flame_effect_data);
	player_flame_effect_data = NULL;
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