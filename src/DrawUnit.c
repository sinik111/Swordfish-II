#include "DrawUnit.h"

#include <wchar.h>

#include "Global.h"
#include "Vector2.h"
#include "TypeDefines.h"

void CreateDrawUnit(DrawUnit* draw_unit, const Vector2* position, const wchar_t* shape, WORD attribute)
{
	draw_unit->id = GenerateID();
	draw_unit->position = *position;
	draw_unit->shape = *shape;
	draw_unit->attribute = attribute;
}