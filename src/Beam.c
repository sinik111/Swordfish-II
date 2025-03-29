#include "Beam.h"

#include <stdlib.h>

#include "MyTime.h"
#include "PlayScene.h"
#include "Vector2.h"
#include "Player.h"
#include "Global.h"

Beam* CreateBeam(Player* player)
{
	Beam* beam = (Beam*)malloc(sizeof(Beam));
	if (beam == NULL)
	{
		return NULL;
	}
	beam->id = GenerateID();
	vec2 adjust = { 62.0f, 0.0f };
	beam->position = AddVector2(&player->position, &adjust);
	beam->duration = 2.0f;
	beam->timer = 0.0f;

	return beam;
}

void UpdateBeam(Beam* beam)
{
	beam->timer += DeltaTime();
	if (beam->timer >= beam->duration)
	{
		free(beam);
		SetBeam(NULL);
	}
}

void RenderBeam(Beam* beam)
{
	RenderShape(&beam->position, shape_beam_center, (int)(beam->timer * 10));
	RenderShape(&beam->position, shape_beam_side, (int)(beam->timer * 10));
}