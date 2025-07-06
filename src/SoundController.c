#include "SoundController.h"

#include <windows.h>
#include <wchar.h>

#include "TypeDefines.h"
#include "fmod.h"
#include "DebugUtility.h"

static FMOD_SYSTEM* fmod_system = NULL;
static FMOD_SOUND* sounds[name_max] = { 0 };
static FMOD_CHANNEL* channels[name_max] = { 0 };

static void LoadSoundFile(char* file_name, SoundName name);

void InitializeSoundController()
{
	FMOD_System_Create(&fmod_system, FMOD_VERSION);

	FMOD_System_Init(fmod_system, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile("TheFatRat - Fire.mp3", intro_music);
	LoadSoundFile("TheFatRat - Arcadia.mp3", play_music);
	LoadSoundFile("Kawaii Side Of Life.mp3", end_music);
	LoadSoundFile("retro-laser-1-236669.mp3", canon_sound);
	LoadSoundFile("machine-gun-burst-43670.mp3", machine_gun_sound);
	LoadSoundFile("laser-shoot-38126.mp3", skill_charge_sound);
	LoadSoundFile("laser-zap-90575.mp3", skill_fire_sound);
	LoadSoundFile("pixel-explosion-319166.mp3", explosion_sound);
}

static void LoadSoundFile(char* file_name, SoundName name)
{
	char path[MAX_PATH] = { 0 };

	GetModuleFileNameA(NULL, path, MAX_PATH);

	size_t path_length = strlen(path);
	BOOL no_file = TRUE;

	for (int i = (int)path_length - 1; i >= 0; --i)
	{
		if (path[i] == '\\')
		{
			path[i] = '\0';

			if (!strcmp("bin", &(path[i + 1])))
			{
				no_file = FALSE;

				break;
			}
		}
	}

	if (no_file)
	{
		DebugLog("파일을 열 수 없습니다. LoadSoundFile()\n");
		return;
	}

	strcat_s(path, MAX_PATH, "\\data\\");
	strcat_s(path, MAX_PATH, file_name);

	FMOD_System_CreateSound(fmod_system, path, FMOD_DEFAULT, NULL, &sounds[name]);
}

void ReleaseSoundController()
{
	for (int i = 0; i < name_max; ++i)
	{
		FMOD_Sound_Release(sounds[i]);
	}

	FMOD_System_Close(fmod_system);
	FMOD_System_Release(fmod_system);

	fmod_system = NULL;
}

void PlayGameSound(SoundName name)
{
	HRESULT r = FMOD_System_PlaySound(fmod_system, sounds[name], NULL, FALSE, &channels[name]);
	DebugLog("%lld\n", r);
	
}

void StopGameSound(SoundName name)
{
	if (channels[name] != NULL)
	{
		FMOD_Channel_Stop(channels[name]);
	}
}

void SetGameSoundVolume(SoundName name, float volume)
{
	if (channels[name] != NULL)
	{
		FMOD_Channel_SetVolume(channels[name], volume);
	}
}

void SetGameSoundLoop(SoundName name)
{
	FMOD_Sound_SetMode(sounds[name], FMOD_LOOP_NORMAL);
}

void UpdateSoundController()
{
	FMOD_System_Update(fmod_system);
}