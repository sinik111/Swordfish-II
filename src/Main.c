#include "Game.h"
#include "MyTime.h"
#include "Global.h"
#include "DebugUtility.h"

int main()
{
	ENABLE_LEAK_CHECK();

	InitializeTime(); // �ð� �ʱ�ȭ.
	UpdateTime(); // ó���� �ѹ� UpdateTime ����.

	InitializeGame(); // ���� �ʱ�ȭ.
	
	while (!g_quit) // ���� ����. g_quit�� 1�� ������ ������ �ݺ�.
	{
		UpdateGame();
		RenderGame();
		CHECK_FPS();
		UpdateTime();
	}

	ReleaseGame(); // �����Ҵ� ���� �� ó��.

	DUMP_LEAKS();
}