#include "Global.h"

int g_quit = 0;

// �ٸ� ���Ͽ��� ���� �������� ���ϰ� static���� ����
static long long g_global_id = 0;

long long GenerateID()
{
	return g_global_id++;
}