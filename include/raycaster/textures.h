#pragma once

#include "common.h"

#define NUM_TEXTURES 9

typedef struct Texture texture_t;

struct Texture{
    uint32_t *buffer;
    int width, height;
    int channels;
};

texture_t *load_wall_textures();
void free_wall_textures(texture_t *wall_textures);