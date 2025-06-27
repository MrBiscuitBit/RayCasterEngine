#pragma once

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define FPS 60
#define FRAME_TIME (1000 / FPS)

#define WND_WIDTH 1920
#define WND_HEIGHT 1200

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

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;