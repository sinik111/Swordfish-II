#pragma once

#include <wchar.h>

#include "Vector2.h"
#include "TypeDefines.h"

typedef struct UIPlayerSkill
{
	long long id;
	Vector2 position;
	int notice_position;
	int notice_key_position;
	float flicker_timer;
	float flicker_rate;
	BOOL flicker;
	wchar_t ui_gauge_shape[30];
	wchar_t ui_notice_shape[40];
	wchar_t ui_notice_key[40];
} UIPlayerSkill;

UIPlayerSkill* CreateUIPlayerSkill();

void UpdateUIPlayerSKill(UIPlayerSkill* ui);

void RenderUIPlayerSKill(UIPlayerSkill* ui);