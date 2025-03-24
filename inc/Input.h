#pragma once

#include "Types.h"

#define VK_ESCAPE         0x1B
#define VK_SPACE          0x20

#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28

void UpdateInput();

BOOL IsKeyDown(int vKey);

BOOL IsKeyPressed(int vKey);

BOOL IsKeyReleased(int vKey);