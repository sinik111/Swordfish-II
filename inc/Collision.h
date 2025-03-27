#pragma once

typedef struct UnionList List;
typedef struct Enemy Enemy;
typedef struct Player Player;
typedef struct Boss Boss;

void CheckBulletsToBossCollision(List* bullet_list, Boss* enemy);

void CheckBulletsToEnemiesCollision(List* bullet_list, List* enemy_list);

void CheckPlayerToEnemyBulletsCollision(Player* player, List* enemy_bullet_list);