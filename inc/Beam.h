#pragma once

#include "Vector2.h"

typedef struct Player Player;

typedef struct Beam
{
	long long id;
	Vector2 position;
	float timer;
	float duration;
} Beam;

Beam* CreateBeam(Player* player);

void UpdateBeam(Beam* beam);

void RenderBeam(Beam* beam);