#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>
#include <stdio.h>
#include <float.h>
#include <raylib.h>
#include <raymath.h>
#include "utils/log.h"


// -- Viewport
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// -- Controls
#define MOVEMENT_SPEED 0.1
#define DEBUG 1

// -- Physics
#define GRAVITY ((Vector3) {0.0, 0.0f, -9.81})
#define ITERATIONS 8 // Verlet interations
#define RADIUS 10