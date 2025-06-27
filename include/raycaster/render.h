#pragma once

#include "common.h"
#include <SDL3\SDL.h> 

// Drawing Functions
void render_pixel(int x, int y, u32 color);
void render_rect(int x, int y, int width, int height, u32 color);

void clear_color_buffer(u32 clear_color);
void render_color_buffer();

// Util Functions
int init_sdl_win(int wnd_width, int wnd_height);
int init_buffer(int width, int height);
int resize_buffer(int width, int height);
void clean_buffer();
void clean_sdl();