#pragma once

#include "../cons.h"
#include "verlet.c"

PhysicData* init_physics();
void free_physic(PhysicData* data);