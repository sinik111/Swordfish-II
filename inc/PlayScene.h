#pragma once

typedef struct UnionList List;

void InitializePlayScene();

void UpdatePlayScene();

void RenderPlayScene();

void ReleasePlayScene();

List* GetEffectList();

List* GetPlayerBulletList();