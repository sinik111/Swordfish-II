#include "Input.h"

#include <windows.h> // GetAsyncKeyState 사용
#include <memory.h>

static SHORT key_state[256] = { 0 };
static SHORT previous_key_state[256] = { 0 };

void UpdateInput()
{
    // 이전 상태 저장
    memcpy_s(previous_key_state, sizeof(previous_key_state), key_state, sizeof(key_state));

    for (int i = 0; i < 256; ++i)
    {
        key_state[i] = GetAsyncKeyState(i);
    }
}

// 키가 눌려진 상태인지
BOOL IsKeyDown(int vKey)
{
    return (key_state[vKey] & 0x8000) != 0;
}

// 이번 루프에 키를 눌렀는지
BOOL IsKeyPressed(int vKey)
{
    return (!(previous_key_state[vKey] & 0x8000) && (key_state[vKey] & 0x8000));
}

// 이번 루프에 키를 뗐는지
BOOL IsKeyReleased(int vKey)
{
    return ((previous_key_state[vKey] & 0x8000) && !(key_state[vKey] & 0x8000));
}