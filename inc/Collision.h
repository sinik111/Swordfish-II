#pragma once

typedef struct UnionList List;
typedef struct Enemy Enemy;
typedef struct Player Player;

void CheckBulletsToEnemyCollision(List* bullet_list, Enemy* enemy);

void CheckBulletsToEnemiesCollision(List* bullet_list, List* enemy_list);

void CheckplayerToEnemyBulletsCollision(Player* player, List* enemy_bullet_list);