#pragma once

typedef struct UnionList List;
typedef struct Enemy Enemy;
typedef struct SwordfishII SwordfishII;

void CheckBulletsToEnemyCollision(List* bullet_list, Enemy* enemy);

void CheckBulletsToEnemiesCollision(List* bullet_list, List* enemy_list);

void CheckSwordfishToEnemyBulletsCollision(SwordfishII* swordfish, List* enemy_bullet_list);