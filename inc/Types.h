#pragma once

typedef int BOOL;
typedef unsigned __int64 size_t;
typedef short SHORT;
typedef unsigned short WORD;

#ifndef TRUE
#define TRUE 1
#endif // TRUE
#ifndef FALSE
#define FALSE 0
#endif // FALSE

#ifndef NULL
#define NULL ((void*)0)
#endif // NULL

typedef enum DataType
{
	PLAYER,
	BULLET,
	ENEMY,
	LIST,
	EFFECT_UNIT,
} DataType;

//typedef enum Layer
//{
//	BACKGROUND,
//	GAMEOBJECT,
//	UI
//} Layer;