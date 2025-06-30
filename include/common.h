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

#define HEX_BLACK        0xFF000000  
#define HEX_WHITE        0xFFFFFFFF  
#define HEX_RED          0xFF0000FF  
#define HEX_GREEN        0xFF00FF00  
#define HEX_BLUE         0xFFFF0000  

#define HEX_YELLOW       0xFF00FFFF  
#define HEX_CYAN         0xFFFFFF00  
#define HEX_MAGENTA      0xFFFF00FF  
#define HEX_LOG_L        0xFF4D4D33   

#define HEX_DARK_RED     0xFF000080  
#define HEX_DARK_GREEN   0xFF008000  
#define HEX_DARK_BLUE    0xFF800000  

#define HEX_GRAY         0xFF808080  
#define HEX_DARK_GRAY    0xFF404040  
#define HEX_LIGHT_GRAY   0xFFBFBFBF  

#define HEX_ALPHA_KEY    0xFFFF00FF  

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;