#pragma once
#include <raylib.h>

struct Propulsion {
   Vector2 maxForceNewtons = {0, 0};
   float   throttlePercent = 0;
};
