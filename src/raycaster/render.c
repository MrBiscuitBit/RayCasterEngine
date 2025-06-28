#include "render.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static struct buffer{
    u32 *color_buffer;
    SDL_Texture *color_buffer_texture;
    int width, height;
}sdl_buffer;

int init_sdl_win(int wnd_width, int wnd_height){

    if(!SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Failed To Initialize SDL");
        return 1;
    }

    if(!SDL_CreateWindowAndRenderer("RayCaster", wnd_width, wnd_height, SDL_WINDOW_RESIZABLE, &window, &renderer)){
        fprintf(stderr, "Failed To Create SDL Window And Renderer");
        SDL_Quit();
        return 2;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return 0;
}

void clean_sdl(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int init_buffer(int width, int height){
    sdl_buffer.color_buffer = (u32 *)malloc(sizeof(u32) * (u32)width * (u32)height);
    if(!sdl_buffer.color_buffer){
        fprintf(stderr, "Failed To Allocate Color Buffer\n");
        return 1;
    }

    sdl_buffer.color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
    if(!sdl_buffer.color_buffer_texture){
        fprintf(stderr, "Failed To Create Color Buffer Texture\n");
        return 2;
    }

    sdl_buffer.width = width;
    sdl_buffer.height = height;
    return 0;
}

int resize_buffer(int width, int height){
    if(width == sdl_buffer.width && height == sdl_buffer.height) return 1;

    sdl_buffer.color_buffer = (u32 *)realloc(sdl_buffer.color_buffer, (u32)sizeof(u32) * (u32)width * (u32)height);
    if(!sdl_buffer.color_buffer){
        fprintf(stderr, "Failed To Reallocate Color Buffer\n");
        return 1;
    }

    sdl_buffer.width = width;
    sdl_buffer.height = height;
    return 0;
}

void clean_buffer(){
    if(sdl_buffer.color_buffer){
        free(sdl_buffer.color_buffer);
        sdl_buffer.color_buffer = NULL;
    }
    if(sdl_buffer.color_buffer_texture){
        SDL_DestroyTexture(sdl_buffer.color_buffer_texture);
        sdl_buffer.color_buffer_texture = NULL;
    }
}

void render_pixel(int x, int y, u32 color){
    if(x >= 0 && x < sdl_buffer.width && y >= 0 && y < sdl_buffer.height)
        sdl_buffer.color_buffer[(sdl_buffer.width * y) + x] = color; 
}

void render_line(int x0, int y0, int x1, int y1, u32 color){
    int delta_x = x1 - x0;
    int delta_y = y1 - y0;

    int side_length = abs(delta_x) >= abs(delta_y)? abs(delta_x): abs(delta_y);

    float x_step = delta_x / (float)side_length;
    float y_step = delta_y / (float)side_length;

    float current_x = x0;
    float current_y = y0;
    for(int i = 0; i < side_length; i++){
        render_pixel(round(current_x), round(current_y), color);
        current_x += x_step;
        current_y += y_step;
    }
}

void render_rect(int x, int y, int width, int height, u32 color){
    for(int i = y; i < y + height; i++){
        for(int j = x; j < x + width; j++){
            if(j < 0 || j > sdl_buffer.width) continue;
            if(i < 0 || i > sdl_buffer.height) continue;
            render_pixel(j, i, color);
        }
    }
}

void clear_color_buffer(u32 clear_color){
    for(int x = 0; x < sdl_buffer.width * sdl_buffer.height; x++){
            sdl_buffer.color_buffer[x] = clear_color;
    }
}

void render_color_buffer(){
    SDL_UpdateTexture(sdl_buffer.color_buffer_texture, NULL, sdl_buffer.color_buffer, (int)((uint32_t)sdl_buffer.width * sizeof(uint32_t)));
    SDL_RenderTexture(renderer, sdl_buffer.color_buffer_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}