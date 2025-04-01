#include "MenuScene.h"

#include <stdlib.h>
#include <wchar.h>

#include "ConsoleRenderer.h"
#include "UnionList.h"
#include "MyTime.h"
#include "Input.h"
#include "Game.h"
#include "MenuBackground.h"
#include "MenuStar.h"
#include "SoundController.h"

static List* star_list = NULL;
static float star_timer = 0.0f;
static float star_rate = 0.05f;

void InitializeMenuScene()
{
	InitializeMenuBackgroundData();

	star_list = CreateList(MENU_SCENE_STAR);

	PlayGameSound(intro_music);

	SetGameSoundLoop(intro_music);

	SetGameSoundVolume(intro_music, 0.05f);
}

void UpdateMenuScene()
{
	star_timer += DeltaTime();

	if (star_timer > star_rate)
	{
		star_timer -= star_rate;

		MenuStar star;
		CreateMenuStar(&star);

		Insert(star_list, &star, sizeof(MenuStar));
	}

	Node* previous_node = NULL;
	Node* current_node = star_list->head;
	while (current_node != NULL)
	{
		UpdateMenuStar(&current_node->data.star);
		if (IsStarDestroyed(&current_node->data.star))
		{
			current_node = RemoveNode(star_list, previous_node, current_node);
		}
		else
		{
			previous_node = current_node;
			current_node = current_node->next;
		}
	}

	if (IsKeyReleased(VK_SPACE))
	{
		ChangeScene(PLAY);
	}
}

void RenderMenuScene()
{
	RenderMenuBackground();

	Node* current_node = star_list->head;
	while (current_node != NULL)
	{
		RenderMenuStar(&current_node->data.star);

		current_node = current_node->next;
	}

	ScreenDrawString(90, 10, L"Swordfish II", FG_WHITE);

	ScreenDrawString(86, 20, L"press SPACE to start", FG_WHITE);
}

void ReleaseMenuScene()
{
	StopGameSound(intro_music);

	ReleaseMenuBackgroundData();

	DeleteList(star_list);
}