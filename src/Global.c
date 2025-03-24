#include "Global.h"

int g_quit = 0;

// 다른 파일에서 직접 접근하지 못하게 static으로 선언
static long long g_global_id = 0;

long long GenerateID()
{
	return g_global_id++;
}