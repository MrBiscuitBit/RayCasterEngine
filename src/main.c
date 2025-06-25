#include "rc_render.h"

int init_sdl_win(int wnd_width, int wnd_height, SDL_Window **win, SDL_Renderer **ren, int fullscreen){

    if(!SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Failed To Initialize SDL");
        return 1;
    }

    if(!SDL_CreateWindowAndRenderer("RayCaster", wnd_width, wnd_height, fullscreen? SDL_WINDOW_FULLSCREEN: 0, win, ren)){
        fprintf(stderr, "Failed To Create SDL Window And Renderer");
        SDL_Quit();
        return 2;
    }
    SDL_SetRenderDrawBlendMode(*ren, SDL_BLENDMODE_BLEND);

    return 0;
}

void process_events(int *running){

    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_EVENT_QUIT){
            *running = 0;
            return;
        }

        if(event.type == SDL_EVENT_KEY_DOWN){
            switch(event.key.scancode){
                case SDL_SCANCODE_ESCAPE:
                    *running = 0;
                    break;
                default:
                    break;
            }
        }
    }

}

void cap_frame_rate(Uint64 frame_start){
    Uint64 target_time = frame_start + FRAME_TIME; // Time To Start The Next Frame
    Uint64 current_time = SDL_GetTicks();

    if(current_time + 1 < target_time){
        SDL_Delay((Uint32)(target_time - current_time - 1));
    }

    while(SDL_GetTicks() < target_time);
}

int main(int argc, char *argv[]){
    puts("Hello RayCaster!");

    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;
    if(init_sdl_win(WND_WIDTH, WND_HEIGHT, &win, &ren, 1)){
        fprintf(stderr, "Failed To Init SDL And Window/Renderer\n");
        return 1;
    }
    
    raycaster_t *ray_caster = malloc(sizeof(raycaster_t));
    if(!ray_caster){
        fprintf(stderr, "Failed To Allocate RayCaster\n");
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    ray_caster->board = rc_load_board("resrcs\\boards\\default_board.txt", 64);
    if(!ray_caster->board){
        fprintf(stderr, "Failed To Load Board\n");
    }

    ray_caster->player = malloc(sizeof(rc_player_t));
    if(!ray_caster->player){
        fprintf(stderr, "Failed To Allocate Player\n");
        rc_clean_up(ray_caster);
    }

    ray_caster->player->pos.x = ray_caster->board->dimensions.x * 0.5f;
    ray_caster->player->pos.y = ray_caster->board->dimensions.y * 0.5f;
    ray_caster->player->width = ray_caster->player->height = 5;
    ray_caster->player->angle = PI * 0.5f;
    ray_caster->player->turn_right = 0; 
    ray_caster->player->turn_right = 0;
    ray_caster->player->walk_forward = 0;
    ray_caster->player->walk_backward = 0;
    ray_caster->player->walk_speed = 100;
    ray_caster->player->turn_speed = 75 * (PI / 180);

    // NoteWorthy Resolutions Are 320x240 And 1440x900
    ray_caster->res = (vec2i_t){1440, 900};
    ray_caster->fov = 75 * (PI / 180);
    ray_caster->ray_count = ray_caster->res.x;
    ray_caster->dist_to_proj = (ray_caster->res.x * 0.5f) / tan(ray_caster->fov * 0.5f);

    ray_caster->rays = malloc(sizeof(ray_t) * ray_caster->ray_count);

    // Allocate Color Buffer And Create Texture For It
    ray_caster->ren = malloc(sizeof(rc_renderer_t));
    ray_caster->ren->color_buffer = (Uint32 *)malloc(sizeof(Uint32) * (Uint32)ray_caster->res.x * (Uint32)ray_caster->res.y);
    ray_caster->ren->color_buffer_texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, ray_caster->res.x, ray_caster->res.y);
    ray_caster->ren->wall_textures = load_wall_textures();

    float delta_time; // Frame Time In Seconds
    Uint64 prev_time, current_time;
    prev_time = SDL_GetTicks();
    const bool *key_state = NULL;

    int running = 1;
    while(running == 1){

        current_time = SDL_GetTicks();
        delta_time = (double)(current_time - prev_time) / 1000.0f;
        prev_time = current_time;

        //printf("DeltaTime: %f\n", delta_time);
        key_state = SDL_GetKeyboardState(NULL);

        // Events/Input
        process_events(&running);
        input_player(ray_caster->player, key_state);

        //Updating
        rc_update(ray_caster, delta_time);

        // Rendering
        SDL_SetRenderDrawColorFloat(ren, CLR_WHITE);
        SDL_RenderClear(ren);

        rc_render_all(ren, ray_caster, 0.2f, 0, 0, 0, 1);

        SDL_RenderPresent(ren);

        cap_frame_rate(current_time);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}