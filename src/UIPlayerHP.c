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
	wmemset(ui->ui_shape, 0, 40);
	//▋

	return ui;
}

void UpdateUIplayerHP(UIplayerHP* ui)
{
	wmemset(ui->ui_shape, 0, 40);

	wmemcpy_s(ui->ui_shape, 3, L"HP ", 3);
	
	Player* player = GetPlayer();
	for (int i = 0; i < player->hp; ++i)
	{
		ui->ui_shape[i + 3] = L'▋';
	}
}

void RenderUIplayerHP(UIplayerHP* ui)
{
	ScreenDrawString((int)ui->position.x, (int)ui->position.y, ui->ui_shape, FG_WHITE);
}