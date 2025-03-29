#include "UIPlayerSkill.h"

#include <wchar.h>
#include <stdlib.h>

#include "ConsoleRenderer.h"
#include "Player.h"
#include "Global.h"
#include "Vector2.h"
#include "DebugUtility.h"
#include "PlayScene.h"
#include "MyTime.h"

UIPlayerSkill* CreateUIPlayerSkill()
{
	UIPlayerSkill* ui = (UIPlayerSkill*)malloc(sizeof(UIPlayerSkill));
	if (ui == NULL)
	{
		DebugLog("동적할당 실패. CreateUIPlayerSkill()");
		return NULL;
	}

	ui->id = GenerateID();
	vec2 position = { 0.0f, 1.0f };
	ui->position = position;
	wmemset(ui->ui_gauge_shape, 0, 30);
	wmemset(ui->ui_notice_shape, 0, 40);

	ui->flicker_timer = 0.0f;
	ui->flicker_rate = 0.2f;

	return ui;
}

void UpdateUIPlayerSKill(UIPlayerSkill* ui)
{
	wmemset(ui->ui_gauge_shape, 0, 30);
	wmemset(ui->ui_notice_shape, 0, 40);
	wmemset(ui->ui_notice_key, 0, 40);

	int index = 0;
	wmemcpy_s(ui->ui_gauge_shape, 7, L"ENERGY ", 7);
	index = 7;

	Player* player = GetPlayer();
	for (int i = 0; i < player->skill_gauge_max; ++i)
	{
		if (i < player->skill_gauge)
		{
			ui->ui_gauge_shape[index++] = L'▋';
		}
		else
		{
			ui->ui_gauge_shape[index++] = L' ';
		}
	}

	ui->ui_gauge_shape[index++] = L' ';

	ui->notice_position = index;

	if (player->skill_gauge_max == player->skill_gauge)
	{
		wmemcpy_s(ui->ui_notice_shape, 13, L"⬤   FULL   ⬤ ", 13);
	}
	else
	{
		wmemcpy_s(ui->ui_notice_shape, 13, L"⬤ CHARGING ⬤ ", 13);
	}
	

	index += 13;

	if (player->skill_gauge_max == player->skill_gauge)
	{
		wmemcpy_s(ui->ui_notice_key, 15, L"press S to fire", 15);

		ui->notice_key_position = index;
	}

	
}

void RenderUIPlayerSKill(UIPlayerSkill* ui)
{
	Player* player = GetPlayer();

	ScreenDrawString((int)ui->position.x, (int)ui->position.y, ui->ui_gauge_shape, FG_BLUE);

	if (player->skill_gauge_max == player->skill_gauge)
	{
		ui->flicker_timer += DeltaTime();
		if (ui->flicker_timer >= ui->flicker_rate)
		{
			ui->flicker_timer -= ui->flicker_rate;

			ui->flicker = !ui->flicker;
		}

		if (ui->flicker)
		{
			ScreenDrawString((int)ui->position.x + ui->notice_position, (int)ui->position.y, ui->ui_notice_shape, FG_YELLOW);
		}
		else
		{
			ScreenDrawString((int)ui->position.x + ui->notice_position, (int)ui->position.y, ui->ui_notice_shape, FG_RED);
		}

		ScreenDrawString((int)ui->position.x + ui->notice_key_position, (int)ui->position.y, ui->ui_notice_key, FG_YELLOW);
	}
	else
	{
		ScreenDrawString((int)ui->position.x + ui->notice_position, (int)ui->position.y, ui->ui_notice_shape, FG_GRAY);
	}
}