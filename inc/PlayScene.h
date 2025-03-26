#pragma once

#include "TypeDefines.h"

typedef struct UnionList List;
typedef struct Player Player;

void InitializePlayScene();

void UpdatePlayScene();

void RenderPlayScene();

void ReleasePlayScene();

List* GetEffectList();

List* GetPlayerBulletList();

List* GetEnemyList();

List* GetEnemyBulletList();

Player* GetPlayer();

BOOL IsEnemyAllDestroyed();