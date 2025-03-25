#pragma once

// float x, float y
typedef struct Vector2
{
	float x;
	float y;
} Vector2;

typedef Vector2 vec2;

// 기본적인 방향 벡터들

extern const Vector2 UpVector; // { 0.0f, -1.0f }
extern const Vector2 DownVector; // { 0.0f, 1.0f }
extern const Vector2 RightVector; // { 1.0f, 0.0f }
extern const Vector2 LeftVector; // { -1.0f, 0.0f }

extern const Vector2 ZeroVector; // { 0.0f, 0.0f }


float GetVecter2Length(const Vector2* v_in); // 벡터의 크기(길이)를 반환함

void NormalizeVector2(Vector2* v_out); // 벡터의 크기(길이)를 1으로 만듦

Vector2 AddVector2(const Vector2* v1_in, const Vector2* v2_in); // v1_in + v2_in 의 결과를 반환함

Vector2 SubVector2(const Vector2* v1_in, const Vector2* v2_in); // v1_in - v2_in 의 결과를 반환함

// count에 가변인자 개수, 가변인자에 float 사용
// v_in의 x, y에 각 인자를 곱한 벡터를 반환함
Vector2 ScalarMulVector2Args(const Vector2* v_in, int count, ...);

Vector2 ScalarMulVector2(const Vector2* v_in, float scalar); // v_in의 x, y에 scalar를 곱한 벡터를 반환함

Vector2 ScalarMulVector2Each(const Vector2* v_in, float x, float y); // v_in의 x, y에 x, y를 곱한 벡터를 반환함

Vector2 MakeDirectionVector2(const Vector2* v1_in, const Vector2* v2_in); // v1_in <-- v2_in 의 방향 벡터를 반환함