#include "DebugUtility.h"

#ifdef _DEBUG

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#include "MyTime.h"

// FPS ���
static int frame_count = 0;
static float frame_timer = 0.0f;
static char fps_buffer[30] = { 0 };

void __CheckFPS()
{
    ++frame_count;

    frame_timer += DeltaTime();
    if (frame_timer > 1.0f)
    {
        sprintf_s(fps_buffer, 30, "FPS: %d\n", frame_count);

        frame_timer -= 1.0f;
        frame_count = 0;

        DebugLog(fps_buffer);
    }
}

// �α� ���
void __PrintDebugLog(const char* fmt, ...)
{
    char buffer[1024];

    va_list args;

    va_start(args, fmt);

    vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, fmt, args);

    va_end(args);

    OutputDebugStringA(buffer);
}

#endif