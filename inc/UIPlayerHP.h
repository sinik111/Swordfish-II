#pragma once

#include <wchar.h>

#include "Vector2.h"

typedef struct Player Player;

typedef struct UIplayerHP
{
	long long id;
	Vector2 position;
	wchar_t ui_shape[40];
} UIplayerHP;

UIplayerHP* CreateUIplayerHP();

void UpdateUIplayerHP(UIplayerHP* ui);

void RenderUIplayerHP(UIplayerHP* ui);