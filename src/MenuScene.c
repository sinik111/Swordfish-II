#include "MenuScene.h"

#include <stdlib.h>
#include <wchar.h>

#include "ConsoleRenderer.h"
#include "UnionList.h"
#include "MyTime.h"

static wchar_t menu_data[5][25];

void InitializeMenuScene()
{
	for (int i = 0; i < 4; ++i)
	{
		wmemcpy_s(menu_data[i], 25, L"menu menu menu menu menu", 25);
	}

	wmemcpy_s(menu_data[4], 25, L"press space to countinue", 25);
}

void UpdateMenuScene()
{

}

void RenderMenuScene()
{

	for (int i = 0; i < 4; ++i)
	{
		ScreenDrawString(ScreenWidth() / 2 - 12, ScreenHeight() / 2 - 3 + i, menu_data[i], FG_BLUE);
	}

	ScreenDrawString(ScreenWidth() / 2 - 12, ScreenHeight() / 2 + 9, menu_data[4], FG_YELLOW);
}

void ReleaseMenuScene()
{

}