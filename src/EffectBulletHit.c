#include "EffectBulletHit.h"

#include <stdlib.h>

#include "UnionList.h"
#include "Vector2.h"
#include "FileLoader.h"
#include "DebugUtility.h"

EffectBulletHitData* CreateEffectBulletHitData()
{
	StringData* sd = LoadData("effect_bullet_hit.txt");

	EffectBulletHitData* effect_bullet_hit_data = (EffectBulletHitData*)malloc(sizeof(EffectBulletHitData));
	if (effect_bullet_hit_data == NULL)
	{
		return NULL;
	}

	List* effect_frame_list = CreateList(LIST);
	effect_bullet_hit_data->effect_frame_list = effect_frame_list;
	effect_bullet_hit_data->frames = sd->count;
	effect_bullet_hit_data->duration = 1.0f;
	
	for (int c = 0; c < sd->count; ++c)
	{
		List* effect_frames = CreateList(EFFECT_UNIT);
		Insert(effect_frame_list, effect_frames, sizeof(List));

		for (int i = 0; i < sd->n; ++i)
		{
			for (int j = 0; j < sd->m; ++j)
			{
				wchar_t c = 0;
				c = *(sd->data + (j * i + i * sd->m + c * sd->m * sd->n));
				if (c != L' ')
				{
					EffectUnit effect_unit;
					Vector2 position = { (float)(j - sd->m / 2), (float)(i - sd->n / 2) };
					CreateEffectUnit(&effect_unit, &position, &c);
					Insert(effect_frames, &effect_unit, sizeof(EffectUnit));
				}
			}
		}
	}
	
	return effect_bullet_hit_data;

	//for (int i = 0; i < sd->count; ++i)
	//{
	//	int a, b, c, d;

	//	int result = swscanf(sd->data + sd->m * sd->n * i, L"%d %d %d %d", &a, &b, &c, &d);

	//	int asdf = 0;
	//}
}