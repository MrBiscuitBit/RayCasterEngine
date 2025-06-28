#include "ray_caster.h"

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

    if(init_sdl_win(WND_WIDTH, WND_HEIGHT)){
        fprintf(stderr, "Failed To Init SDL And Window/Renderer\n");
        return 1;
    }
        
    // NoteWorthy Resolutions Are 320x240 And 1440x900
    raycaster_t *ray_caster = init_raycaster(1440, 900);
    if(!ray_caster){
        fprintf(stderr, "Failed To Init RayCaster\n");
        clean_buffer();
        clean_sdl();
        return 1;
    }

    float delta_time; // Frame Time In Seconds
    Uint64 prev_time, current_time;
    prev_time = SDL_GetTicks();
    const bool *key_state = NULL;

    float map_scale_factor = 0.25;

    int running = 1;
    while(running == 1){

        current_time = SDL_GetTicks();
        delta_time = (double)(current_time - prev_time) / 1000.0f;
        prev_time = current_time;

        //printf("DeltaTime: %f\n", delta_time);
        key_state = SDL_GetKeyboardState(NULL);

        // Events/Input
        process_events(&running);
        rc_input_player(ray_caster->player, key_state);

        //Updating
        rc_update(ray_caster, delta_time);

        // Rendering
        clear_color_buffer(0xFFFFFFFF);

        rc_prepare_wall_projection(ray_caster);
        rc_render_map(ray_caster->board, map_scale_factor);
        //rc_render_rays(ray_caster, map_scale_factor);
        rc_render_player(ray_caster->player, map_scale_factor);

        render_color_buffer();
        
        cap_frame_rate(current_time);
    }

    rc_clean_up(ray_caster);
    clean_buffer();
    clean_sdl();
    return 0;
}