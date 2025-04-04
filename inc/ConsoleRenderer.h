#pragma once

#include <wchar.h>

#include "TypeDefines.h"

// Attributes flags. 이거 쓰려고 windows.h가 여기 include 되어있어서 그냥 빼서 직접 define해둠

#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.

typedef enum FG_Color
{
	FG_BLACK		= 0,
	FG_RED			= FOREGROUND_RED | FOREGROUND_INTENSITY,
	FG_GREEN		= FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	FG_BLUE			= FOREGROUND_BLUE | FOREGROUND_INTENSITY,

	FG_RED_DARK		= FOREGROUND_RED,
	FG_GREEN_DARK	= FOREGROUND_GREEN,
	FG_BLUE_DARK	= FOREGROUND_BLUE,

	FG_YELLOW		= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	FG_SKY			= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	FG_PINK			= FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,

	FG_YELLOW_DARK	= FOREGROUND_RED | FOREGROUND_GREEN,
	FG_SKY_DARK		= FOREGROUND_GREEN | FOREGROUND_BLUE,
	FG_PINK_DARK	= FOREGROUND_RED | FOREGROUND_BLUE,

	FG_GRAY			= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	FG_WHITE		= FG_GRAY | FOREGROUND_INTENSITY
} FG_Color;

typedef enum BG_Color
{
	BG_BLACK		= 0,

	BG_RED			= BACKGROUND_RED | BACKGROUND_INTENSITY,
	BG_GREEN		= BACKGROUND_GREEN | BACKGROUND_INTENSITY,
	BG_BLUE			= BACKGROUND_BLUE | BACKGROUND_INTENSITY,

	BG_RED_DARK		= BACKGROUND_RED,
	BG_GREEN_DARK	= BACKGROUND_GREEN,
	BG_BLUE_DARK	= BACKGROUND_BLUE,

	BG_YELLOW		= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
	BG_SKY			= BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY,
	BG_PINK			= BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY,

	BG_YELLOW_DARK	= BACKGROUND_RED | BACKGROUND_GREEN,
	BG_SKY_DARK		= BACKGROUND_GREEN | BACKGROUND_BLUE,
	BG_PINK_DARK	= BACKGROUND_RED | BACKGROUND_BLUE,

	BG_GRAY			= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	BG_WHITE		= BG_GRAY | BACKGROUND_INTENSITY
} BG_Color;

void ScreenInit();
void ScreenRelease();

void ScreenFlipping();
void ScreenClear();

BOOL ScreenDrawChar(int x, int y, wchar_t ch, WORD attr);
BOOL ScreenDrawString(int x, int y, const wchar_t* pStr, WORD attr);
BOOL ScreenSetAttributes(WORD attr);

int ScreenWidth();
int ScreenHeight();