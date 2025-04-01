#pragma once

#include "TypeDefines.h"

typedef enum SoundName
{
	intro_music,
	play_music,
	end_music,
	canon_sound,
	machine_gun_sound,
	skill_charge_sound,
	skill_fire_sound,
	explosion_sound,
	name_max
} SoundName;

void InitializeSoundController();

void ReleaseSoundController();

void PlayGameSound(SoundName name);

void StopGameSound(SoundName name);

void SetGameSoundVolume(SoundName name, float volume);

void SetGameSoundLoop(SoundName name);