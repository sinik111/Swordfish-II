#pragma once

#include <wchar.h>

#include "Vector2.h"

typedef struct Player Player;

typedef struct UIplayerHP
{
	long long id;
	Vector2 position;
	int index;
	wchar_t ui_hp_shape[30];
	wchar_t ui_shield_shape[20];
} UIplayerHP;

UIplayerHP* CreateUIplayerHP();

void UpdateUIplayerHP(UIplayerHP* ui);

void RenderUIplayerHP(UIplayerHP* ui);