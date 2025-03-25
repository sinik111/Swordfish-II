#include "Shape.h"

#include <stdlib.h>
#include <wchar.h>

#include "Vector2.h"
#include "UnionList.h"
#include "FileLoader.h"
#include "Global.h"
#include "ConsoleRenderer.h"
#include "TypeDefines.h"

static ShapeData* player_shape_data = NULL;
static ShapeData* enemy_1_shape_data = NULL;

static void CreateShapeData(const wchar_t* file_name, ShapeType type);
static void ReleaseShapeDataList(ShapeData* shape_Data);

void InitializeShapeData()
{
	CreateShapeData(L"player_shape.txt", PLAYER_SHAPE);
	CreateShapeData(L"enemy_1_shape.txt", ENEMY_1_SHAPE);
}

static void CreateShapeData(const wchar_t* file_name, ShapeType type)
{
	StringData* sd = LoadSingleLineData(file_name);
	if (sd == NULL)
	{
		return;
	}

	ShapeData* data = (ShapeData*)malloc(sizeof(ShapeData));
	if (data == NULL)
	{
		return;
	}

	switch (type)
	{
	case PLAYER_SHAPE:
		player_shape_data = data;
		break;

	case ENEMY_1_SHAPE:
		enemy_1_shape_data = data;
		break;

	default:
		return;
	}

	List* shape_frame_list = CreateList(LIST);
	shape_frame_list->id = GenerateID();
	data->id = GenerateID();
	data->shape_frame_list = shape_frame_list;
	data->frames = sd->count;
	data->attribute = sd->additional1;

	for (int c = 0; c < sd->count; ++c)
	{
		List* shape_frames = CreateList(DRAW_UNIT);
		shape_frames->id = GenerateID();
		Insert(shape_frame_list, &shape_frames, sizeof(List*));

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
					CreateDrawUnit(&draw_unit, &position, &shape, data->attribute);
					Insert(shape_frames, &draw_unit, sizeof(DrawUnit));
				}
			}
		}
	}

	free(sd->data);
	free(sd);
}

void RenderShape(const Vector2* position, ShapeType type, int frame)
{
	ShapeData* shape_data = NULL;
	switch (type)
	{
	case PLAYER_SHAPE:
		shape_data = player_shape_data;
		break;

	case ENEMY_1_SHAPE:
		shape_data = enemy_1_shape_data;
		break;

	default:
		return;
	}

	List* frame_list = shape_data->shape_frame_list;

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
		Vector2 shape_position = AddVector2(position, &current_shape_node->data.draw_unit.position);
		ScreenDrawChar((int)shape_position.x, (int)shape_position.y, shape, attribute);

		current_shape_node = current_shape_node->next;
	}
}

void ReleaseShapeData()
{
	ReleaseShapeDataList(player_shape_data);
	player_shape_data = NULL;
	ReleaseShapeDataList(enemy_1_shape_data);
	enemy_1_shape_data = NULL;
}

static void ReleaseShapeDataList(ShapeData* shape_Data)
{
	if (shape_Data != NULL)
	{
		Node* current_node = shape_Data->shape_frame_list->head;
		while (current_node != NULL)
		{
			DeleteList(current_node->data.shape_list);

			current_node = current_node->next;
		}

		DeleteList(shape_Data->shape_frame_list);

		free(shape_Data);
	}
}