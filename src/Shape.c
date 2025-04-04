#include "Shape.h"

#include <stdlib.h>
#include <wchar.h>

#include "Vector2.h"
#include "UnionList.h"
#include "FileLoader.h"
#include "Global.h"
#include "ConsoleRenderer.h"
#include "TypeDefines.h"
#include "DebugUtility.h"

static ShapeData* shape_data[SHAPE_MAX] = { 0 };

static void CreateShapeData(const wchar_t* file_name, ShapeName name);
static void ReleaseShapeDataList(ShapeData* shape_data);

void InitializeShapeData()
{
	CreateShapeData(L"shape_player.txt", shape_player);
	CreateShapeData(L"shape_enemy_1.txt", shape_enemy_1);
	CreateShapeData(L"shape_enemy_2.txt", shape_enemy_2);
	CreateShapeData(L"shape_enemy_3.txt", shape_enemy_3);
	CreateShapeData(L"shape_boss_ship.txt", shape_boss_ship);
	CreateShapeData(L"shape_boss_gun.txt", shape_boss_gun);
	CreateShapeData(L"shape_item_center.txt", shape_item_center);
	CreateShapeData(L"shape_item_pink.txt", shape_item_pink);
	CreateShapeData(L"shape_item_yellow.txt", shape_item_yellow);
	CreateShapeData(L"shape_item_sky.txt", shape_item_sky);
	CreateShapeData(L"shape_player_shield.txt", shape_player_shield);
	CreateShapeData(L"shape_beam_center.txt", shape_beam_center);
	CreateShapeData(L"shape_beam_side.txt", shape_beam_side);
}

static void CreateShapeData(const wchar_t* file_name, ShapeName name)
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

	shape_data[name] = data;

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

void RenderShape(const Vector2* position, ShapeName name, int frame)
{
	ShapeData* data = shape_data[name];

	List* frame_list = data->shape_frame_list;

	Node* current_frame_list = frame_list->head;
	while (frame--)
	{
		current_frame_list = current_frame_list->next;
	}

	wchar_t buffer[120] = { 0 };
	int indexer = 0;
	Vector2 previous_position = { -1.0f, 0.0f };
	Vector2 first_position;
	Node* current_shape_node = current_frame_list->data.effect_list->head;
	while (1)
	{
		wchar_t shape = current_shape_node->data.draw_unit.shape;
		WORD attribute = current_shape_node->data.draw_unit.attribute;
		Vector2 shape_position = AddVector2(position, &current_shape_node->data.draw_unit.position);

		if ((int)shape_position.x >= 0 && (int)shape_position.y >= 0 &&
			(int)shape_position.x < ScreenWidth() && (int)shape_position.y < ScreenHeight())
		{
			if ((int)previous_position.x == -1)
			{
				buffer[indexer++] = shape;
				first_position.x = shape_position.x;
				first_position.y = shape_position.y;
				previous_position.x = shape_position.x;
				previous_position.y = shape_position.y;
			}
			else if ((int)shape_position.x == (int)previous_position.x + 1 && (int)shape_position.y == (int)previous_position.y)
			{
				buffer[indexer++] = shape;
				previous_position.x = shape_position.x;
				previous_position.y = shape_position.y;
			}
			else
			{
				buffer[indexer] = L'\0';
				indexer = 0;
				ScreenDrawString((int)first_position.x, (int)first_position.y, buffer, attribute);

				buffer[indexer++] = shape;
				first_position.x = shape_position.x;
				first_position.y = shape_position.y;
				previous_position.x = shape_position.x;
				previous_position.y = shape_position.y;
			}
		}
		else if (indexer)
		{
			buffer[indexer] = L'\0';
			indexer = 0;
			previous_position.x = -1.0f;
			ScreenDrawString((int)first_position.x, (int)first_position.y, buffer, attribute);
		}

		current_shape_node = current_shape_node->next;

		if (current_shape_node == NULL)
		{
			if (indexer)
			{
				buffer[indexer] = L'\0';
				ScreenDrawString((int)first_position.x, (int)first_position.y, buffer, attribute);
			}

			break;
		}
	}
}

void ReleaseShapeData()
{
	for (int i = 0; i < SHAPE_MAX; ++i)
	{
		ReleaseShapeDataList(shape_data[i]);
		shape_data[i] = NULL;
	}
}

static void ReleaseShapeDataList(ShapeData* shape_data)
{
	if (shape_data != NULL)
	{
		Node* current_node = shape_data->shape_frame_list->head;
		while (current_node != NULL)
		{
			DeleteList(current_node->data.shape_list);

			current_node = current_node->next;
		}

		DeleteList(shape_data->shape_frame_list);

		free(shape_data);
	}
}