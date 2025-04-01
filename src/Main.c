#include "Game.h"
#include "MyTime.h"
#include "Global.h"
#include "DebugUtility.h"

#pragma comment(lib, "fmod_vc.lib")

int main()
{
	ENABLE_LEAK_CHECK();

	InitializeTime(); // 시간 초기화.
	UpdateTime(); // 처음에 한번 UpdateTime 해줌.

	InitializeGame(); // 게임 초기화.
	
	while (!g_quit) // 게임 루프. g_quit에 1이 들어오기 전까지 반복.
	{
		UpdateGame();
		RenderGame();
		CHECK_FPS();
		UpdateTime();
	}

	ReleaseGame(); // 동적할당 해제 등 처리.

	DUMP_LEAKS();
}