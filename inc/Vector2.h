#pragma once

// float x, float y
typedef struct Vector2
{
	float x;
	float y;
} Vector2;

typedef Vector2 vec2;

// �⺻���� ���� ���͵�

extern const Vector2 UpVector; // { 0.0f, -1.0f }
extern const Vector2 DownVector; // { 0.0f, 1.0f }
extern const Vector2 RightVector; // { 1.0f, 0.0f }
extern const Vector2 LeftVector; // { -1.0f, 0.0f }

extern const Vector2 ZeroVector; // { 0.0f, 0.0f }


float GetVecter2Length(const Vector2* v_in); // ������ ũ��(����)�� ��ȯ��

void NormalizeVector2(Vector2* v_out); // ������ ũ��(����)�� 1���� ����

Vector2 AddVector2(const Vector2* v1_in, const Vector2* v2_in); // v1_in + v2_in �� ����� ��ȯ��

Vector2 SubVector2(const Vector2* v1_in, const Vector2* v2_in); // v1_in - v2_in �� ����� ��ȯ��

// count�� �������� ����, �������ڿ� float ���
// v_in�� x, y�� �� ���ڸ� ���� ���͸� ��ȯ��
Vector2 ScalarMulVector2Args(const Vector2* v_in, int count, ...);

Vector2 ScalarMulVector2(const Vector2* v_in, float scalar); // v_in�� x, y�� scalar�� ���� ���͸� ��ȯ��

Vector2 MakeDirectionVector2(const Vector2* v1_in, const Vector2* v2_in); // v1_in <-- v2_in �� ���� ���͸� ��ȯ��