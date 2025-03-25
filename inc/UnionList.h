#pragma once

#include "TypeDefines.h"

#include "Bullet.h"
#include "Enemy.h"
#include "DrawUnit.h"
#include "Effect.h"
#include "EnemySpawner.h"

typedef enum DataType
{
	PLAYER,
	BULLET,
	ENEMY,
	LIST,
	DRAW_UNIT,
	EFFECT,
	ENEMY_SPAWN_DATA
} DataType;

// 리스트에 여러가지 타입들을 담을 수 있게 union을 이용함.
typedef struct UnionNode
{
	union
	{
		// 각 타입의 struct들도 첫번째 멤버로 long long 형 id를 가지고 있음
		// union이라서 struct 생성후 리스트에 넣을 시 자동으로 채워짐
		// 삭제나 id에 따른 특정 구현을 할 때 타입을 몰라도 id를 알 수 있음
		long long id;
		Bullet bullet;
		Enemy enemy;
		List* effect_list;
		List* shape_list;
		DrawUnit draw_unit;
		Effect effect;
		EnemySpawnData spawn_data;
	} data;
	struct UnionNode* next;
} Node;

// 리스트에는 타입을 담은 변수를 가지고
// 무슨 타입을 보관중인지 확인
typedef struct UnionList
{
	long long id;
	Node* head;
	Node* tail;
	DataType type;
} List;

List* CreateList(DataType type);

void DeleteList(List* list);

void Insert(List* list, void* data, size_t data_size);

Node* Remove(List* list, long long id);

Node* RemoveNode(List* list, Node* previous, Node* remove);

void ClearList(List* list);