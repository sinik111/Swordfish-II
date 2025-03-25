#pragma once

#include <wchar.h>

typedef struct StringData
{
	int m;
	int n;
	int count;
	wchar_t* data;
} StringData;

StringData* LoadData(const char* file_name);