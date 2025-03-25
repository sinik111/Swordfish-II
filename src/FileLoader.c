#include "FileLoader.h"

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "DebugUtility.h"

StringData* LoadData(const char* file_name)
{
	char path[MAX_PATH] = { 0 };

	GetModuleFileNameA(NULL, path, MAX_PATH);

	int path_length = (int)strlen(path);
	int no_file = 1;

	for (int i = path_length - 1; i >= 0; --i)
	{
		if (path[i] == '\\')
		{
			path[i] = '\0';

			if (!strcmp("bin", &(path[i + 1])))
			{
				no_file = 0;

				break;
			}
		}
	}

	if (no_file)
	{
		DebugLog("파일을 열 수 없습니다. LoadData()\n");
		return NULL;
	}

	strcat_s(path, MAX_PATH, "\\data\\");
	strcat_s(path, MAX_PATH, file_name);

	FILE* fp = fopen(path, "r");
	if (fp == NULL)
	{
		DebugLog("파일을 열 수 없습니다. LoadData()\n");
		return NULL;
	}

	StringData* sd = (StringData*)malloc(sizeof(StringData*));
	if (sd == NULL)
	{
		fclose(fp);
		DebugLog("sd 할당 실패. LoadData()\n");
		return NULL;
	}

	fscanf_s(fp, "%d", &sd->m);
	fscanf_s(fp, "%d", &sd->n);
	fscanf_s(fp, "%d", &sd->count);
	fgetwc(fp);

	sd->data = (wchar_t*)malloc(sizeof(wchar_t) * sd->m * sd->n * sd->count);
	if (sd->data == NULL)
	{
		fclose(fp);
		printf("sd->data 할당 실패. LoadData()\n");
		return NULL;
	}

	wmemset(sd->data, 0, sd->m * sd->n * sd->count);

	for (int i = 0; i < sd->count; ++i)
	{
		int position = sd->m * sd->n * i;
		fgetws(sd->data + position, sd->m * sd->n + 2, fp);
	}

	fclose(fp);

	return sd;
}