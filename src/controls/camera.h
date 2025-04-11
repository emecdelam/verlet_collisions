#pragma once

#include "../cons.h"


Camera3D* init_camera();
void move_camera(Vector3 direction);
void move_target(Vector3 target);