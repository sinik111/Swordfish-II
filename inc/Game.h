#pragma once

typedef enum Scene
{
	NONE,
	MENU,
	PLAY,
	END
} Scene;

void InitializeGame();

void UpdateGame();

void RenderGame();

void ReleaseGame();

void ChangeScene(Scene scene);