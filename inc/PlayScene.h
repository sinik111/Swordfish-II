#pragma once

#include "TypeDefines.h"

typedef struct UnionList List;
typedef struct Player Player;
typedef struct Boss Boss;
typedef struct Beam Beam;

void InitializePlayScene();

void UpdatePlayScene();

void RenderPlayScene();

void ReleasePlayScene();

List* GetEffectList();

List* GetPlayerBulletList();

List* GetEnemyList();

List* GetEnemyBulletList();

Player* GetPlayer();

Boss* GetBoss();

void SetBeam(Beam* new_beam);