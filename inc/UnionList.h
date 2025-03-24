#pragma once

#include "Types.h"

#include "Bullet.h"
#include "Enemy.h"

// ����Ʈ�� �������� Ÿ�Ե��� ���� �� �ְ� union�� �̿���.
typedef struct UnionNode
{
	union
	{
		// �� Ÿ���� struct�鵵 ù��° ����� long long �� id�� ������ ����
		// union�̶� struct ������ ����Ʈ�� ���� �� �ڵ����� ä����
		// ������ id�� ���� Ư�� ������ �� �� Ÿ���� ���� id�� �� �� ����
		long long id;
		Bullet bullet;
		Enemy enemy;
	} data;
	struct UnionNode* next;
} Node;

// ����Ʈ���� Ÿ���� ���� ������ ������
// ���� Ÿ���� ���������� Ȯ��
typedef struct UnionList
{
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