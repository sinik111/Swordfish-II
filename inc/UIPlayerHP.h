#pragma once

#include <wchar.h>

#include "Vector2.h"

typedef struct UIPlayerHP
{
	long long id;
	Vector2 position;
	int index;
	wchar_t ui_hp_shape[30];
	wchar_t ui_shield_shape[20];
} UIPlayerHP;

UIPlayerHP* CreateUIPlayerHP();

void UpdateUIPlayerHP(UIPlayerHP* ui);

void RenderUIPlayerHP(UIPlayerHP* ui);