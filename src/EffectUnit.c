#include "EffectUnit.h"

#include <wchar.h>

#include "Global.h"
#include "Vector2.h"

void CreateEffectUnit(EffectUnit* effect_unit, const Vector2* position, const wchar_t* shape)
{
	effect_unit->id = GenerateID();
	effect_unit->position = *position;
	effect_unit->shape = *shape;
}