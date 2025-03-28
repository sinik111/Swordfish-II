#include "UIPlayerHP.h"

#include <wchar.h>
#include <stdlib.h>

#include "ConsoleRenderer.h"
#include "Player.h"
#include "Global.h"
#include "Vector2.h"
#include "DebugUtility.h"
#include "PlayScene.h"

UIplayerHP* CreateUIplayerHP()
{
	UIplayerHP* ui = (UIplayerHP*)malloc(sizeof(UIplayerHP));
	if (ui == NULL)
	{
		DebugLog("동적할당 실패. CreateUIplayerHP()");
		return NULL;
	}

	ui->id = GenerateID();
	ui->position = ZeroVector;
	wmemset(ui->ui_hp_shape, 0, 30);
	wmemset(ui->ui_shield_shape, 0, 20);

	return ui;
}

void UpdateUIplayerHP(UIplayerHP* ui)
{
	wmemset(ui->ui_hp_shape, 0, 30);
	wmemset(ui->ui_shield_shape, 0, 20);

	ui->index = 0;
	wmemcpy_s(ui->ui_hp_shape, 3, L"HP ", 3);
	ui->index = 3;
	
	Player* player = GetPlayer();
	for (int i = 0; i < player->hp; ++i)
	{
		ui->ui_hp_shape[ui->index++] = L'▋';
	}
	ui->ui_hp_shape[ui->index] = L'\0';

	int shield_index = 0;
	while (shield_index < player->shield)
	{
		ui->ui_shield_shape[shield_index++] = L'▋';
	}
	ui->ui_shield_shape[shield_index] = L'\0';
}

void RenderUIplayerHP(UIplayerHP* ui)
{
	ScreenDrawString((int)ui->position.x, (int)ui->position.y, ui->ui_hp_shape, FG_WHITE);
	ScreenDrawString((int)ui->position.x + ui->index, (int)ui->position.y, ui->ui_shield_shape, FG_SKY);
}