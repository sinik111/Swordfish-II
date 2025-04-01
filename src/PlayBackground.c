#include "PlayBackground.h"

#include <stdlib.h>
#include <wchar.h>

#include "FileLoader.h"
#include "Vector2.h"
#include "MyTime.h"
#include "TypeDefines.h"
#include "ConsoleRenderer.h"
#include "DebugUtility.h"

static int data_n = 0;
static int data_m = 0;
static WORD data_attribute = 0;
static int position = 0;
static float timer = 0.0f;
static float speed = 0.05f;
static wchar_t** data = NULL;

static void CreatePlayBackgroundData(const wchar_t* file_name);

void InitializePlayBackgroundData()
{
	data_n = 0;
	data_m = 0;
	data_attribute = 0;
	position = 0;
	timer = 0.0f;
	speed = 0.05f;

	CreatePlayBackgroundData(L"shape_play_background.txt");
}

void CreatePlayBackgroundData(const wchar_t* file_name)
{
	StringData* sd = LoadSingleLineData(file_name);
	if (sd == NULL)
	{
		return;
	}

	data_n = sd->n;
	data_m = sd->m;
	data_attribute = sd->additional1;

	data = (wchar_t**)malloc(sizeof(wchar_t*) * sd->n);
	if (data == NULL)
	{
		return;
	}

	for (int i = 0; i < sd->n; ++i)
	{
		data[i] = (wchar_t*)malloc(sizeof(wchar_t) * sd->m);
		if (data[i] == NULL)
		{
			return;
		}

		wmemcpy(data[i], sd->data + i * sd->m, sd->m);
	}

	free(sd->data);
	free(sd);
}

void UpdatePlayBackground()
{
	timer += DeltaTime();
	if (timer >= speed)
	{
		timer -= speed;
		++position;
		if (position == data_m)
		{
			position = 0;
		}
	}
}

void RenderPlayBackground()
{
	for (int i = 0; i < data_n; ++i)
	{
		wchar_t buffer[121] = { 0 };

		if (position + 120 > data_m)
		{
			// 1100
			// 20 = 1100 + 120 - 1200
			int remain = position + 120 - data_m;

			wmemcpy(buffer, data[i] + position, 120 - remain);
			wmemcpy(buffer + (120 - remain), data[i], remain);
		}
		else
		{
			wmemcpy(buffer, data[i] + position, 120);
		}
		
		ScreenDrawString(0, i, buffer, data_attribute);
	}
}

void ReleasePlayBackgroundData()
{
	if (data != NULL)
	{
		for (int i = 0; i < data_n; ++i)
		{
			free(data[i]);
		}

		free(data);
		data = NULL;
	}
}