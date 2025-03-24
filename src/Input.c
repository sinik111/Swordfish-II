#include "Input.h"

#include <windows.h> // GetAsyncKeyState ���
#include <memory.h>

static SHORT key_state[256] = { 0 };
static SHORT previous_key_state[256] = { 0 };

void UpdateInput()
{
    // ���� ���� ����
    memcpy_s(previous_key_state, sizeof(previous_key_state), key_state, sizeof(key_state));

    for (int i = 0; i < 256; ++i)
    {
        key_state[i] = GetAsyncKeyState(i);
    }
}

// Ű�� ������ ��������
BOOL IsKeyDown(int vKey)
{
    return (key_state[vKey] & 0x8000) != 0;
}

// �̹� ������ Ű�� ��������
BOOL IsKeyPressed(int vKey)
{
    return (!(previous_key_state[vKey] & 0x8000) && (key_state[vKey] & 0x8000));
}

// �̹� ������ Ű�� �ô���
BOOL IsKeyReleased(int vKey)
{
    return ((previous_key_state[vKey] & 0x8000) && !(key_state[vKey] & 0x8000));
}