#include "EndScene.h"

#include <wchar.h>

#include "ConsoleRenderer.h"
#include "Game.h"
#include "MyTime.h"

static wchar_t end_data[10];
static float end_timer;

void InitializeEndScene()
{
	wmemset(end_data, 0, 10);
	wmemcpy_s(end_data, 4, L"end5", 4);

	end_timer = 0.0f;
}

void UpdateEndScene()
{
	end_timer += DeltaTime();

	if (end_timer >= 5.0f)
	{
		ChangeScene(MENU);
	}

	for (int i = 0; i < (int)end_timer; ++i)
	{
		end_data[3 + i] = L'.';
		end_data[3 + i + 1] = L'4' - i;
	}
}

void RenderEndScene()
{
	ScreenDrawString(ScreenWidth() / 2 - 2, ScreenHeight() / 2 - 1, end_data, FG_GREEN);
}

void ReleaseEndScene()
{

}