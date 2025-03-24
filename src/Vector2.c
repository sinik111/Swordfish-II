#include "Vector2.h"

#include <math.h>
#include <stdarg.h>

const Vector2 UpVector = { 0.0f, -1.0f };
const Vector2 DownVector = { 0.0f, 1.0f };
const Vector2 RightVector = { 1.0f, 0.0f };
const Vector2 LeftVector = { -1.0f, 0.0f };

const Vector2 ZeroVector = { 0.0f, 0.0f };

float GetVecter2Length(const Vector2* v_in) // ������ ũ��(����)�� ��ȯ��
{
	if (v_in != NULL)
	{
		// ��Ÿ����� ���� �̿�
		return sqrtf(v_in->x * v_in->x + v_in->y * v_in->y);
	}

	return 0.0f;
}

void NormalizeVector2(Vector2* v_out) // ������ ũ��(����)�� 1���� ����
{
	if (v_out != NULL)
	{
		float length = GetVecter2Length(v_out);
		if (length > 0.0f)
		{
			v_out->x /= length;
			v_out->y /= length;
		}
	}
}

Vector2 AddVector2(const Vector2* v1_in, const Vector2* v2_in) // v1_in + v2_in �� ����� ��ȯ��
{
	Vector2 v = { v1_in->x + v2_in->x, v1_in->y + v2_in->y };

	return v;
}

Vector2 SubVector2(const Vector2* v1_in, const Vector2* v2_in) // v1_in - v2_in �� ����� ��ȯ��
{
	Vector2 v = { v1_in->x - v2_in->x, v1_in->y - v2_in->y };

	return v;
}

// count�� �������� ����, �������ڿ� float ���
// v_in�� x, y�� �� ���ڸ� ���� ���͸� ��ȯ��
Vector2 ScalarMulVector2Args(const Vector2* v_in, int count, ...)
{
	Vector2 v = { v_in->x, v_in->y };

	// ���� ���� ���
	va_list args;

	va_start(args, count);

	for (int i = 0; i < count; ++i)
	{
		// �������ڴ� float�� double�� �ޱ� ������
		// double������ ������ float�� ĳ�����ؼ� �����
		float scalar = (float)va_arg(args, double); 
		v.x *= scalar;
		v.y *= scalar;
	}

	va_end(args);

	return v;
}

Vector2 ScalarMulVector2(const Vector2* v_in, float scalar) // v_in�� x, y�� scalar�� ���� ���͸� ��ȯ��
{
	Vector2 v = { v_in->x, v_in->y };
	v.x *= scalar;
	v.y *= scalar;

	return v;
}

Vector2 MakeDirectionVector2(const Vector2* v1_in, const Vector2* v2_in) // v1_in <-- v2_in �� ���� ���͸� ��ȯ��
{
	// �� - �� �ϸ� �ڿ��� �������� ���� ���Ͱ� ����
	Vector2 direction = SubVector2(v1_in, v2_in);

	NormalizeVector2(&direction);

	return direction;
}

