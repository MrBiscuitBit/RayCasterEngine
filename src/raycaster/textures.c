#include "textures.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

static const char* texture_paths[NUM_TEXTURES] = {
    // Wall Textures
    "./resrcs/wall_textures/redbrick.png",    // [0]
    "./resrcs/wall_textures/purplestone.png", // [1]
    "./resrcs/wall_textures/mossystone.png",  // [2]
    "./resrcs/wall_textures/graystone.png",   // [3]
    "./resrcs/wall_textures/colorstone.png",  // [4]
    "./resrcs/wall_textures/bluestone.png",   // [5]
    "./resrcs/wall_textures/wood.png",        // [6]
    "./resrcs/wall_textures/eagle.png",       // [7]
    "./resrcs/wall_textures/pikuma.png",      // [8] 

    // Sprite Textures
    "resrcs/sprites/barrel.png",              // [9]
    "resrcs/sprites/light.png",               // [10]
    "resrcs/sprites/table.png",               // [11]
    "resrcs/sprites/guard.png",               // [12]
    "resrcs/sprites/armor.png"                // [13]
};

texture_t *load_textures(int *texture_count){
    texture_t *textures = (texture_t *)malloc(sizeof(texture_t) * NUM_TEXTURES);
    if(!textures){
        fprintf(stderr, "Failed To Allocate Wall Textures Array\n");
        return NULL;
    }

    for(int i = 0; i < NUM_TEXTURES; i++){
        textures[i].buffer = (uint32_t *)stbi_load(
            texture_paths[i],
            &textures[i].width,
            &textures[i].height,
            &textures[i].channels,
            4
        );
        textures[i].aspect_ratio = textures[i].width / textures[i].height;
        //if(wall_textures[i].buffer) printf("Loaded Texture: '%s'\n", wall_texture_paths[i]);
    }
    *texture_count = NUM_TEXTURES;

    return textures;
}

void free_textures(texture_t *wall_textures){
    for(int i = 0; i < NUM_TEXTURES; i++){
        stbi_image_free(wall_textures[i].buffer);
    }
}