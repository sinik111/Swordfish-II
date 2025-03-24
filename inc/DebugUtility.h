#pragma once

#ifdef _DEBUG

// �޸� ���� üũ
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define ENABLE_LEAK_CHECK() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#define DUMP_LEAKS() _CrtDumpMemoryLeaks()

// FPS üũ
#define CHECK_FPS() __CheckFPS()
// �Լ��� ���� ���� �ȵ�
void __CheckFPS();

// �α� ���
// ���־� ��Ʃ���->����->���->����׿� �α� ��� ��.
// printf�� �Ȱ��� ����ϸ� ��.
#define DebugLog(...) __PrintDebugLog(__VA_ARGS__)
// �Լ��� ���� ���� �ȵ�
void __PrintDebugLog(const char* fmt, ...);

#else // !_DEBUG

#define ENABLE_LEAK_CHECK() ((void)0)
#define DUMP_LEAKS() ((void)0)
#define CHECK_FPS() ((void)0)
#define DebugLog(...) ((void)0)

#endif // _DEBUG