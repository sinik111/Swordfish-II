#include "DrawUnit.h"

#include <wchar.h>

#include "Global.h"
#include "Vector2.h"

void CreateDrawUnit(DrawUnit* draw_unit, const Vector2* position, const wchar_t* shape)
{
	draw_unit->id = GenerateID();
	draw_unit->position = *position;
	draw_unit->shape = *shape;
}