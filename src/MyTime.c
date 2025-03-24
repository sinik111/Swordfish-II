#include "MyTime.h"

#include <windows.h>

static LARGE_INTEGER frequency;
static LARGE_INTEGER previous_counter;

static float delta_time = 0.f;

void InitializeTime()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&previous_counter);

	srand((unsigned int)previous_counter.QuadPart);
}

void UpdateTime()
{
	LARGE_INTEGER current_counter;

	QueryPerformanceCounter(&current_counter);

	delta_time = (float)(current_counter.QuadPart - previous_counter.QuadPart) / frequency.QuadPart;

	previous_counter = current_counter;
}

float DeltaTime()
{
	return delta_time;
}