#pragma once

#include "common.h"

#define NUM_TEXTURES 14

typedef struct Texture texture_t;

struct Texture{
    uint32_t *buffer;
    int width, height;
    float aspect_ratio; // width / height
    int channels;
};

texture_t *load_textures();
void free_textures(texture_t *wall_textures);