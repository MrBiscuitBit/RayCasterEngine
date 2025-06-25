#include "textures.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

static const char* wall_texture_paths[NUM_TEXTURES] = {
    "./resrcs/wall_textures/redbrick.png",
    "./resrcs/wall_textures/purplestone.png",
    "./resrcs/wall_textures/mossystone.png",
    "./resrcs/wall_textures/graystone.png",
    "./resrcs/wall_textures/colorstone.png",
    "./resrcs/wall_textures/bluestone.png",
    "./resrcs/wall_textures/wood.png",
    "./resrcs/wall_textures/eagle.png",
    "./resrcs/wall_textures/pikuma.png"
};

texture_t *load_wall_textures(){
    texture_t *wall_textures = (texture_t *)malloc(sizeof(texture_t) * NUM_TEXTURES);
    if(!wall_textures){
        fprintf(stderr, "Failed To Allocate Wall Textures Array\n");
        return NULL;
    }

    for(int i = 0; i < NUM_TEXTURES; i++){
        wall_textures[i].buffer = (uint32_t *)stbi_load(
            wall_texture_paths[i],
            &wall_textures[i].width,
            &wall_textures[i].height,
            &wall_textures[i].channels,
            4
        );
        //if(wall_textures[i].buffer) printf("Loaded Texture: '%s'\n", wall_texture_paths[i]);
    }

    return wall_textures;
}

void free_wall_textures(texture_t *wall_textures){
    for(int i = 0; i < NUM_TEXTURES; i++){
        stbi_image_free(wall_textures[i].buffer);
    }
}