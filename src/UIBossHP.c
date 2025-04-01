#include "UIBossHp.h"

#include <wchar.h>
#include <stdlib.h>

#include "ConsoleRenderer.h"
#include "Player.h"
#include "Global.h"
#include "Vector2.h"
#include "DebugUtility.h"
#include "PlayScene.h"
#include "Boss.h"

UIBossHP* CreateUIBossHP()
{
	UIBossHP* ui = (UIBossHP*)malloc(sizeof(UIBossHP));
	if (ui == NULL)
	{
		return NULL;
	}

	ui->id = GenerateID();
	ui->position.x = 65.0f;
	ui->position.y = 0.0f;
	wmemset(ui->ui_hp_shape, 0, 60);

	return ui;
}

void UpdateUIBossHP(UIBossHP* ui)
{
	// 119
	// BOSS - 5
	// 60
	// 65
	// 54
	wmemset(ui->ui_hp_shape, 0, 60);

	ui->index = 0;
	wmemcpy_s(ui->ui_hp_shape, 5, L"BOSS ", 5);
	ui->index = 5;

	Boss* boss = GetBoss();
	int count = (int)(50 * (float)boss->hp / boss->max_hp);
	if (count == 0 && (float)boss->hp / boss->max_hp > 0)
	{
		count = 1;
	}

	for (int i = 0; i < count; ++i)
	{
		ui->ui_hp_shape[ui->index++] = L'▋';
	}
}

void RenderUIBossHP(UIBossHP* ui)
{
	ScreenDrawString((int)ui->position.x, (int)ui->position.y, ui->ui_hp_shape, FG_RED);
}