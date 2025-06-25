#pragma once

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define WND_WIDTH 320
#define WND_HEIGHT 240

#define CLR_BLACK        0.0f, 0.0f, 0.0f, 1.0f
#define CLR_WHITE        1.0f, 1.0f, 1.0f, 1.0f
#define CLR_RED          1.0f, 0.0f, 0.0f, 1.0f
#define CLR_GREEN        0.0f, 1.0f, 0.0f, 1.0f
#define CLR_BLUE         0.0f, 0.0f, 1.0f, 1.0f

#define CLR_YELLOW       1.0f, 1.0f, 0.0f, 1.0f
#define CLR_CYAN         0.0f, 1.0f, 1.0f, 1.0f
#define CLR_MAGENTA      1.0f, 0.0f, 1.0f, 1.0f
#define CLR_LOGL         0.2f, 0.3f, 0.3f, 1.0f

#define CLR_DARK_RED     0.5f, 0.0f, 0.0f, 1.0f
#define CLR_DARK_GREEN   0.0f, 0.5f, 0.0f, 1.0f
#define CLR_DARK_BLUE    0.0f, 0.0f, 0.5f, 1.0f

#define CLR_GRAY         0.5f, 0.5f, 0.5f, 1.0f
#define CLR_DARK_GRAY    0.25f, 0.25f, 0.25f, 1.0f
#define CLR_LIGHT_GRAY   0.75f, 0.75f, 0.75f, 1.0f

#define CLR_TRANSPARENT  0.0f, 0.0f, 0.0f, 0.0f