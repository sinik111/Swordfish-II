#pragma once

#include <wchar.h>

#include "Vector2.h"

typedef struct UIBossHP
{
	long long id;
	Vector2 position;
	int index;
	wchar_t ui_hp_shape[60];
} UIBossHP;

UIBossHP* CreateUIBossHP();

void UpdateUIBossHP(UIBossHP* ui);

void RenderUIBossHP(UIBossHP* ui);