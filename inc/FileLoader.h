#pragma once

#include <wchar.h>

typedef struct StringData
{
	int m;
	int n;
	int count;
	int additional1;
	int additional2;
	int additional3;
	wchar_t* data;
} StringData;

StringData* LoadSingleLineData(const wchar_t* file_name);

StringData* LoadMultiLineData(const wchar_t* file_name);