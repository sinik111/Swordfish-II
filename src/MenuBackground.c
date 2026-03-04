#include "MenuBackground.h"

#include "DebugUtility.h"

#include <stdlib.h>
#include <wchar.h>

#include "FileLoader.h"
#include "Vector2.h"
#include "MyTime.h"
#include "TypeDefines.h"
#include "ConsoleRenderer.h"

static int data_n = 0;
static int data_m = 0;
static WORD data_attribute = 0;
static wchar_t** data = NULL;

static void CreateMenuBackgroundData(const wchar_t* file_name);

void InitializeMenuBackgroundData()
{
	CreateMenuBackgroundData(L"shape_menu_background.txt");
}

void CreateMenuBackgroundData(const wchar_t* file_name)
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

void RenderMenuBackground()
{
	for (int i = 0; i < data_n; ++i)
	{
		wchar_t buffer[121] = { 0 };

		wmemcpy(buffer, data[i], data_m);

		ScreenDrawString(0, i, buffer, data_attribute);
	}

	if (rand() % 2 == 0)
	{
		ScreenDrawString(10, 20, L"✧✦", rand() % 15 + 1);
		ScreenDrawString(8, 21, L"✧✦✧✦", rand() % 15 + 1);
		ScreenDrawString(6, 22, L"✧✦✧✦✧✦✧", rand() % 15 + 1);
		ScreenDrawString(4, 23, L"✧✦✧✦✧✦✧✦✧", rand() % 15 + 1);
		ScreenDrawString(2, 24, L"✧✦✧✦✧✦✧✦✧✦✧", rand() % 15 + 1);
		ScreenDrawString(0, 25, L"✧✦✧✦✧✦✧✦✧✦✧✦✧", rand() % 15 + 1);
		ScreenDrawString(0, 26, L"✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧", rand() % 15 + 1);
		ScreenDrawString(0, 27, L"✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧✦", rand() % 15 + 1);
		ScreenDrawString(0, 28, L"✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧", rand() % 15 + 1);
		ScreenDrawString(0, 29, L"✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧", rand() % 15 + 1);
	}
	else
	{
		ScreenDrawString(10, 20, L"✦✧", rand() % 15 + 1);
		ScreenDrawString(8, 21, L"✦✧✦✧", rand() % 15 + 1);
		ScreenDrawString(6, 22, L"✦✧✦✧✦✧✦", rand() % 15 + 1);
		ScreenDrawString(4, 23, L"✦✧✦✧✦✧✦✧✦", rand() % 15 + 1);
		ScreenDrawString(2, 24, L"✦✧✦✧✦✧✦✧✦✧✦", rand() % 15 + 1);
		ScreenDrawString(0, 25, L"✦✧✦✧✦✧✦✧✦✧✦✧✦", rand() % 15 + 1);
		ScreenDrawString(0, 26, L"✦✧✦✧✦✧✦✧✦✧✦✧✦✧✦", rand() % 15 + 1);
		ScreenDrawString(0, 27, L"✦✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧", rand() % 15 + 1);
		ScreenDrawString(0, 28, L"✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧", rand() % 15 + 1);
		ScreenDrawString(0, 29, L"✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧✦✧", rand() % 15 + 1);
	}
	
}

void ReleaseMenuBackgroundData()
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