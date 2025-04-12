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
#define GRAVITY ((Vector3) {0.0, -9.81, 0.0})
#define ITERATIONS 8 // Verlet iterations for links
#define RADIUS 0.1
#define STEP_TIME 0.20 // Step time

// -- Environment
#define BORDER 1.0
#define BOUNCE 0.5

// -- Rendering
#define MAX_SPEED 0.05
#define MIN_SPEED 0.0
#define OUTLINE 0.01
